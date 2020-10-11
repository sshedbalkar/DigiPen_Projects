#include "Client.h"
#include "../../common/ReturnCodes.h"
#include <iostream>
#include "../../utility/StreamFileSplitter.h"
#include "../../utility/Utility.h"
#include "../../utility/StreamFileJoiner.h"
#include "../../debugTools/InputConsole.h"
#include "../../networking/TCPConnection.h"
#include "../../networking/UDPConnection.h"
#include "CommandCodes.h"
#include "../../debugTools/Assert.h"
#include <iomanip>
#include "../../common/FunctionBind.h"
#include "../../threading/MsgQueue.h"
#include "../../threading/Thread.h"
#include "../../threading/ThreadManager.h"
#include "../../message/Message.h"
//#include "../../os/WindowsIncludes.h"
//
/////////////////////////////////////////
struct ConfigData
{
	std::string		ServerIP;
	unsigned short	ServerPort;
	unsigned short	ClientPort;
	std::string		RecvDir;
	std::string		SendDir;
};
//
struct TargetData
{
	TargetData();
	std::string		TargetIP;
	unsigned short	TargetPort;
	std::string		FileName;
	std::string		ID;
	long long		Size;
};
/////////////////////////////////////////
TargetData::TargetData()
	:TargetIP( "" ),
	TargetPort( 0 ),
	FileName( "" ),
	ID( "-1" )
{
	//
}
/////////////////////////////////////////
//
Client::Client()
	:_TCPConn( 0 ),
	_UDPConn( 0 ),
	_status( ST_Invalid ),
	_input( 0 ),
	_delThreadCounter( 0 ),
	_activeDls( 0 ),
	_activeUps( 0 )
{

}
Client::~Client()
{
	delete _UDPConn;
	delete _TCPConn;
	delete _input;
	::DeleteCriticalSection( &_crit );
	delete _config;
}
int Client::Init( const char *configFile )
{
	_config = new ConfigData();
	ReadConfigFile( configFile, _config );
	CreateSendFileList( _config->SendDir.c_str() );
	_config->RecvDir.append( "/" );
	//
	int ret;
	_TCPConn = new TCPConnection();
	if( (ret = _TCPConn->Init()) != RET_SUCCESS )
	{
		return ret;
	}
	_TCPConn->BindLocal( 0 );
	//
	_UDPConn = new UDPConnection();
	if( (ret = _UDPConn->Init()) != RET_SUCCESS )
	{
		std::cout<<"UDP init was not successful\n";
		return ret;
	}
	_UDPConn->BindLocal( _config->ClientPort );
	unsigned long m2 = 1L;
	_UDPConn->BlockMode( m2 );
	//
	_input = new debug::InputConsole();
	_input->Init();
	//
	::InitializeCriticalSection( &_crit );
	CRITICAL_SECTION *crit = &_crit;
	_downloadThread = ThreadManager::GetSingleton().CreateNewThread( (void*)this, Bind( &Client::DownloaderThread ), crit, 0 );
	//
	_status = ST_Connecting;
	return RET_SUCCESS;
}
int Client::Run( void )
{
	if( ++_delThreadCounter == 100 )
	{
		_delThreadCounter = 0;
		ThreadManager::GetSingleton().Cleanup();
	}
	//
	switch( _status )
	{
	case ST_Invalid:
		{
			//
		}
		break;
	case ST_Connecting:
		{
			SocketAddress addr = TCPConnection::CreateSocketAddrFromData( _config->ServerIP.c_str(), _config->ServerPort );
			std::cout<<"Connecting to Server: "<<addr<<"\n";
			if( _TCPConn->ConnectToRemote( addr ) == RET_SUCCESS )
			{
				std::cout<<"Successfully connected to the server\n";
				unsigned long m = 1L;
				_TCPConn->BlockMode( m );
				//
				_status = ST_NewlyConnected;
				//
				std::cout<<"---------------------------------------------------\n";
			}
			else
			{
				std::cout<<"Do you want to retry? (Y/N): ";
				if( _input->UserInputTrue() == 0 )
				{
					_status = ST_Error;
				}
			}
		}
		break;
	case ST_NewlyConnected:
		{
			std::string srv;
			int ret = ReadServerMsgBlocking( srv );
			if( ret == RET_SUCCESS )
			{
				//std::cout<<"Server: "<<srv.c_str()<<": Len: "<<srv.size()<<"\n";
				//utility::PrintASCII( srv.c_str() );
				//std::cout<<"\n";
				//
				if( !srv.compare(CMD_PORT_ID) )
				{
					SendPortID();
				}
			}
			else
			{
				std::cout<<"Server has disconnected1\n";
				_status = ST_Error;
			}
			_status = ST_Menu;
		}
		break;
	case ST_Error:
		{
			if( _activeDls < 1 && _activeUps < 1 )
			{
				return RET_SERVER_OFFLINE;
			}
		}
		break;
	case ST_Menu:
		{
			std::cout<<"\n";
			std::cout<<"Type '/show' to see a list of available files for download\n";
			std::cout<<"Type '/q' to quit\n";
			_status = ST_WaitingForUser;
		}
		break;
	case ST_WaitingForUser:
		{
			std::string choice;
			//std::getline(std::cin, choice);
			if( _input->GetNextLine( choice ) )
			{
				utility::ToLower( choice );
				std::vector< std::string > input;
				utility::SplitString( choice, ' ', input );
				std::string *cmd = &input.at( 0 );
				if( !cmd->compare("/show") )
				{
					std::cout<<"Showing file list...\n";
					if( _masterList.size() > 0 )
					{
						//ShowMasterList();
						GetMasterShareList();
					}
					else
					{
						GetMasterShareList();
					}
				}
				else if( !cmd->compare("/dl"))
				{
					if( input.size() > 1 )
					{
						int fid = ::atoi( input.at( 1 ).c_str() );
						--fid;
						if( fid > -1 && fid < _masterList.size() )
						{
							FileIPPair &p = _masterList.at( fid );
							//std::cout<<"You want to download: "<<p.first.c_str()<<": "<<p.second.c_str()<<"\n";
							RequestToDownloadFile( p );
						}
						else
						{
							std::cout<<"Invalid file ID\n";
						}
					}
					else
					{
						std::cout<<"You must provide an ID of the file\n";
					}
				}
				else if( !cmd->compare("/q"))
				{
					_status = ST_Error;
				}
				else
				{
					std::cout<<"Invalid choice\n";
				}
			}
			else
			{
				std::string srv;
				int ret;
				if( (ret = ReadServerMsg(srv)) == RET_SUCCESS )
				{
					//std::cout<<"Server: "<<srv.c_str()<<"\n";
					std::vector< std::string > vec;
					utility::SplitString( srv, DELIMITER[0], vec );
					//
					std::string *cmd = &vec.at( 0 );
					if( !cmd->compare(CMD_SHARE_LIST) )
					{
						SendShareList();
					}
					else if( !cmd->compare(CMD_A_SHARE_LIST) )
					{
						AddToMasterList( vec );
					}
					else if( !cmd->compare(CMD_SENT_MASTER_LIST) )
					{
						ShowMasterList();
					}
					else if( !cmd->compare(CMD_UPLOAD_FILE) )
					{
						/*
						0 - C8
						1 - 1
						2 - 192.168.1.136:50087
						3 - 9000
						4 - New Text Document.txt
						5 - 1234
						*/
						UploadFile(vec.at(2), vec.at(4), vec.at(1), vec.at(5), vec.at(3) );
					}
					else if( !cmd->compare(CMD_DOWNLOAD_FILE) )
					{
						DownloadFile(vec.at(2), vec.at(4), vec.at(1), vec.at(5), vec.at(3) );
					}
					else if( !cmd->compare(CMD_NO_LONGER_AVAILABLE) )
					{
						std::cout<<"The file you requested is no longer available for download. Please type '/show' to see the updated list of files.\n";
					}
					else if( !cmd->compare(CMD_NO_FILES_TO_DOWNLOAD) )
					{
						std::cout<<"There are no files available for download at this time.\n";
					}
				}
				else if( ret == RET_SERVER_OFFLINE )
				{
					std::cout<<"1Server is dc'd\n";
					_status = ST_Error;
				}
			}
		}
		break;
	case ST_MasterList:
		{

		}
		break;
	default:
		{

		}
	}
	return RET_SUCCESS;
}
int Client::DownloadFile( const std::string &uploader, const std::string &file, const std::string &id, const std::string &size, const std::string &uport )
{
	std::cout<<"Downloading: "<<file.c_str()<<", From: "<<uploader.c_str()<<", Port: "<<uport.c_str()<<", ID: "<<id.c_str()<<", Size: "<<size.c_str()<<"\n";
	//
	std::vector< std::string > vec;
	utility::SplitString( uploader, ':', vec );
	unsigned short port = ( unsigned short )::atoi( uport.c_str() );
	//std::cout<<"IP: "<<vec.at( 0 ).c_str()<<", Port: "<<port<<"\n";
	TargetData *td = new TargetData();
	td->TargetIP.assign( vec.at(0).c_str() );
	td->TargetPort = port;
	td->FileName.assign( file );
	td->ID.assign( id );
	td->Size = ::atol( size.c_str() );
	//std::cout<<"IP: "<<td->TargetIP.c_str()<<", Port: "<<td->TargetPort<<", File: "<<td->FileName.c_str()<<"\n";
	//
	::EnterCriticalSection( &_crit );
	++_activeDls;
	::LeaveCriticalSection( &_crit );
	//Thread *th = ThreadManager::GetSingleton().CreateNewThread( (void*)this, Bind( &Client::DownloaderThread ), crit, (void*)td );
	Message *m = new Message( MSG_ADD_NEW_DOWNLOAD, td );
	_downloadThread->Messages()->Push( m );
	//
	return RET_SUCCESS;
}
int Client::UploadFile( const std::string &downloader, const std::string &file, const std::string &id, const std::string &size, const std::string &uport )
{
	std::cout<<"Uploading: "<<file.c_str()<<", To: "<<downloader.c_str()<<", Port: "<<uport.c_str()<<", id: "<<id<<", Size: "<<size.c_str()<<"\n";
	std::string str;
	GetFullFileName( file, str );
	//std::cout<<"Full: "<<str.c_str()<<"\n";
	//
	std::vector< std::string > vec;
	utility::SplitString( downloader, ':', vec );
	unsigned short port = ( unsigned short )::atoi( uport.c_str() );
	//std::cout<<"IP: "<<vec.at( 0 ).c_str()<<", Port: "<<port<<"\n";
	TargetData *td = new TargetData();
	td->TargetIP.assign( vec.at(0).c_str() );
	td->TargetPort = port;
	td->FileName.assign( str );
	td->ID.assign( id );
	td->Size = ::atol( size.c_str() );
	//std::cout<<"IP: "<<td->TargetIP.c_str()<<", Port: "<<td->TargetPort<<", File: "<<td->FileName.c_str()<<"\n";
	//
	::EnterCriticalSection( &_crit );
	++_activeUps;
	::LeaveCriticalSection( &_crit );
	CRITICAL_SECTION *crit = &_crit;
	Thread *th = ThreadManager::GetSingleton().CreateNewThread( (void*)this, Bind( &Client::UploaderThread ), crit, (void*)td );
	//
	return RET_SUCCESS;
}
int Client::UploaderThread( void *data, Thread *thread )
{
	TargetData *td = static_cast< TargetData* >( data );
	std::cout<<"UploaderThread IP: "<<td->TargetIP.c_str()<<", Port: "<<td->TargetPort<<", File: "<<td->FileName.c_str()<<"\n";
	//
	sockaddr_in remote = TCPConnection::CreateSockAddr_InFromData( td->TargetIP.c_str(), td->TargetPort ), remote2;
	//
	StreamFileSplitter splitter;
	FileChunk chunk;
	splitter.Init( td->FileName.c_str(), CHUNK_SIZE );
	//
	char size[ 8 ];
	char cid[ 8 ];
	char buffer[ 32 ];
	//
	std::string updata( UDP_BUFF_LEN, ' ' ), ack;
	int ret, ret2;
	std::vector< std::string > vec;
	int lastID = -1;
	bool proceed;
	//
	while( splitter.ReadRandomChunk(&chunk, lastID+1 ) == RET_SUCCESS )
	{
		if( !(thread->Messages()->IsEmpty()) )
		{
			Message *msg = thread->Messages()->Front();
			if( msg->ID == MSG_QUIT )
			{
				return RET_SUCCESS;
			}
			else
			{

			}
			delete msg;
		}
		//
		updata.clear();
		proceed = false;
		//std::cout<<chunk.ID<<": "<<chunk.Payload<<"\n";
		::_itoa_s( chunk.Size, size, 8, 10 );
		::_itoa_s( chunk.ID, cid, 8, 10 );
		//
		updata.assign( CMD_FILE_CHUNK );
		updata.append( DELIMITER );
		updata.append( td->ID );
		updata.append( DELIMITER );
		updata.append( cid );
		updata.append( DELIMITER );
		updata.append( size );
		updata.append( DELIMITER );
		updata.append( chunk.Payload, chunk.Size );
		//
		ret = SendUDPMsg( updata, remote );
		//std::cout<<"Updata size: "<<updata.size()<<", ret: "<<ret<<"\n";
		//::Sleep( 5000 );
		do
		{
			do
			{
				ret2 = UDPConnection::ReceiveFrom( _UDPConn->Socket(), buffer, 32, &remote2 );
			}
			while( TCPConnection::WouldBlock(ret2) );
			if( TCPConnection::IsConnClosed(ret2) || TCPConnection::IsConnReset(ret2) )
			{
				std::cout<<"User has disconnected1\n";
			}
			else
			{
				vec.clear();
				ack.assign( buffer, ret2 );
				//std::cout<<"Got ack: "<<ack.c_str()<<"\n";
				utility::SplitString( ack, DELIMITER[0], vec, 4 );
				if( !vec.at(0).compare(CMD_ACK) )
				{
					if( !vec.at(1).compare(td->ID) )
					{
						lastID = ::atoi( vec.at(2).c_str() );
						proceed = true;
					}
				}
			}
		}
		while( !proceed );
		//
		delete chunk.Payload;
	}
	//
	updata.clear();
	updata.assign( CMD_DONE );
	updata.append( DELIMITER );
	updata.append( td->ID );
	//
	SendUDPMsg( updata, remote );
	//
	thread->StartSync();
	--_activeUps;
	thread->EndSync();
	//
	delete td;
	return RET_SUCCESS;
}
int Client::DownloaderThread( void *data, Thread *thread )
{
	sockaddr_in remoteAddress;
	int ret, ret2;
	std::vector< std::string > vec;
	FileChunk ch;
	std::string recvd( UDP_BUFF_LEN, ' ' ), ack, *id, *cid;
	//
	bool run = true;
	while( run )
	{
		if( !(thread->Messages()->IsEmpty()) )
		{
			Message *msg = thread->Messages()->Front();
			if( msg->ID == MSG_QUIT )
			{
				run = false;
			}
			else
			{
				HandleDownloaderMsg( msg );
			}
			delete msg;
		}
		else if( !_downloadMap.empty() )
		{
			::SecureZeroMemory( &remoteAddress, sizeof(remoteAddress) );
			ret = UDPConnection::ReceiveFrom( _UDPConn->Socket(), _UDPRecvBuff, UDP_BUFF_LEN, &remoteAddress );
			//std::cout<<"Ret: "<<ret<<"\n";
			if( TCPConnection::WouldBlock(ret) )
			{
				//::Sleep( 1 );
			}
			else if( TCPConnection::IsConnClosed(ret) || TCPConnection::IsConnReset(ret) )
			{
				std::cout<<"User has disconnected0\n";
				//run = false;
				//return RET_USER_QUIT;
			}
			else
			{
				vec.clear();
				recvd.clear();
				recvd.assign( _UDPRecvBuff, ret );
				utility::SplitString( recvd, DELIMITER[0], vec, 4 );
				//std::cout<<"Remote: "<<remoteAddress<<"\n";
				if( !vec.at(0).compare(CMD_FILE_CHUNK) )
				{
					id = &vec.at(1);
					DownloadMap::iterator it = _downloadMap.find( *id );
					if( it != _downloadMap.end() )
					{
						cid = &vec.at(2);
						ack.assign( CMD_ACK );
						ack.append( DELIMITER );
						ack.append( *id );
						ack.append( DELIMITER );
						ack.append( *cid );
						ret2 = SendUDPMsg( ack, remoteAddress );
						//
						std::string *s = &vec.at(4);
						int sz = s->size();
						ch.ID = ::atoi( cid->c_str() );
						ch.Size = ::atoi( vec.at(3).c_str() );
						//std::cout<<"ret: "<<ret<<", sz: "<<sz<<", size: "<<ch.Size<<"\n";
						//std::cout<<"Adding chunk: "<<ch.ID<<", sz: "<<sz<<", size: "<<ch.Size<<": "<<s->c_str()<<"\n";
						ch.Payload = new char[ sz+1 ];
						s->copy( ch.Payload, sz, 0 );
						it->second.Joiner->AddChunk(ch);
					}
				}
				else if( !vec.at(0).compare(CMD_DONE) )
				{
					DownloadMap::iterator it = _downloadMap.find( vec.at(1) );
					if( it != _downloadMap.end() )
					{
						std::cout<<"Completed downloading: "<<it->second.Name.c_str()<<"\n";
						it->second.Joiner->Flush();
						_downloadMap.erase( it );
						//
						thread->StartSync();
						--_activeDls;
						thread->EndSync();
					}
				}
			}
		}
		else
		{
			::Sleep( 1 );
			continue;
		}
	}
	//
	return RET_SUCCESS;
}
int Client::HandleDownloaderMsg( Message *msg )
{
	//std::cout<<"Handling Downloader Msg: "<<msg->GetName()<<"\n";
	switch( msg->ID )
	{
	case MSG_ADD_NEW_DOWNLOAD:
		{
			TargetData *td = static_cast< TargetData* >( msg->Obj );
			//
			std::string file( _config->RecvDir );
			file.append( td->FileName );
			//
			StreamFileJoiner *joiner = new StreamFileJoiner();
			joiner->Init( file.c_str(), td->Size );
			//
			DownloadData dd;
			dd.Joiner = joiner;
			dd.Name.assign( td->FileName );
			_downloadMap.insert( DownloadPair(td->ID, dd) );
			//
			std::cout<<"Started Downloading: "<<td->FileName.c_str()<<"\n";
			delete td;
		}
		break;
	default:
		{

		}
	}
	//
	return RET_SUCCESS;
}
int Client::RequestToDownloadFile( FileIPPair &pair )
{
	std::string str( CMD_RQST_TO_DOWNLOAD_FILE );
	str.append( DELIMITER );
	str.append( pair.first );
	str.append( DELIMITER );
	str.append( pair.second.first );
	str.append( DELIMITER );
	str.append( pair.second.second );
	//
	SendServerMsg( str );
	return RET_SUCCESS;
}
/*
C4
192.168.1.136:49747
4
7f9fbb7e0815670834a1310ce1ed8be0.jpg
1234
Lina.flv
1234
New Text Document (2).txt
1234
New Text Document.txt
1234
*/
int Client::AddToMasterList( std::vector< std::string > &alist )
{
	std::string &ip = alist.at( 1 );
	int count = ::atoi( alist.at(2).c_str() ), sz1 = _masterList.size();
	//
	for( std::vector< std::string >::iterator it1 = alist.begin() + 3, it2 = alist.end(), it3; it1 != it2; it1 += 2 )
	{
		it3 = it1 + 1;
		_masterList.push_back( FileIPPair(ip, FileSizePair(*it1, *it3)) );
	}
	//
	MCR_Assert( _masterList.size() == (sz1 + count), "A Share list has error: %s", ip.c_str() );
	//
	return RET_SUCCESS;
}
int Client::ShowMasterList( void )
{
	std::cout.setf( std::ios_base::left );
	std::cout<<std::setfill( '-' )<<std::setw( 79 )<<"-"<<std::setfill( ' ' )<<"\n";
	std::cout<<std::setw( 5 )<<"ID"<<std::setw( 18 )<<"IP"<<std::setw( 40 )<<"FILE"<<std::setw( 15 )<<"SIZE"<<"\n";
	std::cout<<std::setfill( '-' )<<std::setw( 79 )<<"-"<<std::setfill( ' ' )<<"\n";
	//
	int i = 0;
	//std::string *str1;
	std::vector< std::string > vec;
	for( std::vector< FileIPPair >::iterator it1 = _masterList.begin(), it2 = _masterList.end(); it1 != it2; ++it1 )
	{
		utility::SplitString( it1->first, ':', vec, 1 );
		std::cout<<std::setw( 5 )<<++i<<std::setw( 18 )<<vec.at(0).c_str()<<std::setw( 40 )<<it1->second.first.c_str()<<std::setw( 15 )<<it1->second.second.c_str()<<"\n";
		//std::cout<<++i<<": "<<vec.at( 0 ).c_str()<<": "<<it1->second.c_str()<<"\n";
	}
	std::cout<<std::setfill( '-' )<<std::setw( 79 )<<"-"<<std::setfill( ' ' )<<"\n";
	std::cout.unsetf( std::ios_base::left );
	//
	std::cout<<"\nType '/dl <ID of the file>' to download a file, ex: /dl 2\n";
	//
	return RET_SUCCESS;
}
int Client::GetMasterShareList( void )
{
	SendServerMsg( CMD_MASTER_LIST );
	_masterList.clear();
	return RET_SUCCESS;
}
int Client::SendPortID( void )
{
	std::string share( CMD_PORT_ID );
	share.append( DELIMITER );
	//
	char port[ 6 ];
	::_itoa_s( _config->ClientPort, port, 6, 10 );
	share.append( port );
	//
	return SendServerMsg( share );
}
int Client::SendShareList( void )
{
	std::string share( CMD_SHARE_LIST );
	share.append( DELIMITER );
	//
	char total[ 4 ];
	::_itoa_s( _sendList.size(), total, 4, 10 );
	share.append( total );
	//
	char size[ 16 ];
	//
	for each( utility::FileInfo f in _sendList )
	{
		share.append( DELIMITER );
		share.append( f.Name );
		share.append( DELIMITER );
		::_itoa_s( f.Size, size, 16, 10 );
		share.append( size );
	}
	//
	//std::cout<<"Share List: "<<share.c_str()<<"\n";
	return SendServerMsg( share );
}
int Client::ReadServerMsgBlocking( std::string &str )
{
	int ret;
	do
	{
		ret = ReadServerMsg( str );
	}
	while( str.empty() );
	//
	return ret;
}
int Client::ReadServerMsg( std::string &str )
{
	int ret = TCPConnection::Receive( _TCPConn->Socket(), _buffer, BUFF_LEN );
	if( TCPConnection::WouldBlock(ret) )
	{
		//
	}
	else if( TCPConnection::IsConnClosed(ret) || TCPConnection::IsConnReset(ret) )
	{
		std::cout<<"Server has disconnected0\n";
		return RET_SERVER_OFFLINE;
	}
	else
	{
		str.assign( _buffer, ret - 1 );
		//std::cout<<"[Server]: "<<str.c_str()<<", Len: "<<ret<<"\n";
		return RET_SUCCESS;
	}
	//
	return ret;
}
int Client::SendUDPMsg( const char *str, sockaddr_in &addr )
{
	return SendUDPMsg( std::string(str), addr );
}
int Client::SendUDPMsg( const std::string &str, sockaddr_in &addr )
{
	//int start = 0, len = 0, ret;
	//
	/*
	// code to send typed data as 255 byte chunks
	do
	{
		len = str.copy( _UDPSendBuff, BUFF_LEN-1, start );
		_UDPSendBuff[len] = '\0';
		start += len;
		//
		//std::cout<<"Sending: "<<_buffer<<"\n";
		ret = UDPConnection::SendTo( _UDPConn->Socket(), _UDPSendBuff, ++len, &addr );
	}
	while( len == BUFF_LEN );
	*/
	return UDPConnection::SendTo( _UDPConn->Socket(), str.c_str(), str.size(), &addr );
	//return RET_SUCCESS;
}
int Client::SendServerMsg( const char *str )
{
	return SendServerMsg( std::string(str) );
}
int Client::SendServerMsg( const std::string &str )
{
	int start = 0, len = 0, ret;
	//
	// code to send typed data as 255 byte chunks
	do
	{
		len = str.copy( _buffer, BUFF_LEN-1, start );
		_buffer[len] = '\0';
		start += len;
		//
		//std::cout<<"Sending: "<<_buffer<<"\n";
		ret = TCPConnection::Send( _TCPConn->Socket(), _buffer, ++len );
	}
	while( len == BUFF_LEN );
	return RET_SUCCESS;
}
int Client::CreateSendFileList( const char *dir )
{
	utility::FileInfoList vec;
	std::string dir2( dir );
	dir2.append( "/*.*" );
	if( utility::ListFilesInDirectory( dir2.c_str(), vec ) == RET_SUCCESS )
	{
		for each( utility::FileInfo f in vec )
		{
			std::string str2( dir );
			str2.append( "/" );
			str2.append( f.Name );
			if( utility::CanOpenFile(str2.c_str()) )
			{
				_sendList.push_back( utility::FileInfo(f.Name.c_str(), f.Size, str2.c_str()) );
			}
			else
			{
				std::cout<<"Error opening file: "<<str2.c_str()<<", excluding it from the sharing list\n";
			}
		}
	}
	//
	return RET_SUCCESS;
}
int Client::GetFullFileName( const std::string &file, std::string &full )
{
	for each( utility::FileInfo f in _sendList )
	{
		if( !f.Name.compare(file) )
		{
			full.assign( f.FullName );
			break;
		}
	}
	//
	return RET_SUCCESS;
}
int Client::ReadConfigFile( const char *config, ConfigData *data )
{
	FILE *file;
	const int BuffLen = 100;
	char line[ BuffLen ];
	//
	if ( ::fopen_s(&file, config, "r") != 0 )
	{
		std::cout << "[ERROR: Client: 1]: "<<config<<" could not be opened for reading\n" << "\n";
		return RET_FAIL;
	}
	//
	if( utility::ReadNextLine( line, BuffLen, file) != RET_SUCCESS )
	{
		::fclose( file );
		return RET_FAIL;
	}
	data->ServerIP.assign( line );
	data->ServerIP.erase( --data->ServerIP.end() );
	//
	if( utility::ReadNextLine( line, BuffLen, file) != RET_SUCCESS )
	{
		::fclose( file );
		return RET_FAIL;
	}
	data->ServerPort = ::atoi( line );
	//
	if( utility::ReadNextLine( line, BuffLen, file) != RET_SUCCESS )
	{
		::fclose( file );
		return RET_FAIL;
	}
	data->ClientPort = ::atoi( line );
	//
	if( utility::ReadNextLine( line, BuffLen, file) != RET_SUCCESS )
	{
		::fclose( file );
		return RET_FAIL;
	}
	data->RecvDir.assign( line );
	data->RecvDir.erase( --data->RecvDir.end() );
	//
	if( utility::ReadNextLine( line, BuffLen, file) != RET_SUCCESS )
	{
		::fclose( file );
		return RET_FAIL;
		//data->SendDir.erase( --data->SendDir.end() );
	}
	data->SendDir.assign( line );
	//
	::fclose( file );
	//std::cout<<"Server: ["<<data->ServerIP.c_str()<<": "<<data->ServerPort<<"], Client: "<<data->ClientPort<<", Recv: "<<data->RecvDir.c_str()<<", Send: "<<data->SendDir.c_str()<<"\n";
	return RET_SUCCESS;
}