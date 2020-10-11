#include "Server.h"
#include "../../common/ReturnCodes.h"
#include <iostream>
#include "../../utility/FileData.h"
#include "../../networking/TCPConnection.h"
#include "../../threading/Thread.h"
#include "../../common/FunctionBind.h"
#include "../../threading/ThreadManager.h"
#include "../../threading/MsgQueue.h"
#include "../../debugTools/InputConsole.h"
#include "../../utility/Utility.h"
#include "../../message/Message.h"
#include "../../Client/client/CommandCodes.h"
//
//////////////////////////////////////////
//
struct ConfigData
{
	std::string		ServerIP;
	unsigned short	ServerPort;
	unsigned short	ClientPort;
	std::string		RecvDir;
	std::string		SendDir;
};
//
struct FileTransaction
{
	FileTransaction();
	~FileTransaction();
	FileTransaction( const FileTransaction &orig );
	//
	std::string		Uploader;
	std::string		Downloader;
	std::string		FileName;
	int				ID;
	std::string		Size;
	std::string		Port;
};
//
//////////////////////////////////////////
FileTransaction::FileTransaction()
{

}
FileTransaction::~FileTransaction()
{

}
FileTransaction::FileTransaction( const FileTransaction &orig )
	:Uploader( orig.Uploader ),
	Downloader( orig.Downloader ),
	FileName( orig.FileName ),
	ID( orig.ID ),
	Size( orig.Size ),
	Port( orig.Port )
{
	//
}
//////////////////////////////////////////
//
Server::Server()
	:_TCPConn( 0 ),
	_input( 0 ),
	_msgs( 0 ),
	_delThreadCounter( 0 ),
	_uniqueID( 0 )
{
	//
}
Server::~Server()
{
	delete _msgs;
	delete _TCPConn;
	delete _input;
	::DeleteCriticalSection( &_crit );
}
int Server::Init( const char *configFile )
{
	ConfigData data;
	ReadConfigFile( configFile, &data );
	//
	_TCPConn = new TCPConnection();
	int ret;
	if( (ret = _TCPConn->Init()) != RET_SUCCESS )
	{
		return ret;
	}
	SocketAddress addr = TCPConnection::CreateSocketAddrFromData( data.ServerIP.c_str(), data.ServerPort );
	_TCPConn->BindLocal( addr.Port );
	//
	::InitializeCriticalSection( &_crit );
	//
	_input = new debug::InputConsole();
	_input->Init();
	//
	_msgs = new MsgQueue();
	_msgs->Init();
	//
	unsigned long m = 1L;
	_TCPConn->BlockMode( m );
	int i = _TCPConn->Listen();
	std::cout<<"---------------------------------------------------------------\n";
	//
	return RET_SUCCESS;
}
int Server::Run( void )
{
	_input->ExecuteNextCommand();
	if( !(_msgs->IsEmpty()) )
	{
		Message *msg = _msgs->Front();
		HandleMainMsg( msg );
		delete msg;
	}
	//
	ClientSocket	*newSock	= 0;
	int ret;
	if( (ret = _TCPConn->Accept( newSock )) != RET_SUCCESS )
	{
		//Sleep( 500 );
		if( TCPConnection::WouldBlock(ret) )
		{
			//std::cout<<"
		}
		else
		{
			std::cout<<"Connection Accept Fail: "<<ret<<"\n";
			return RET_FAIL;
		}
	}
	else
	{
		User *newUser = new User( newSock );
		_users.insert( UserIDPair(newUser->GetName(), newUser) );
		newUser->Status = ST_NewlyConnected;
		//
		CRITICAL_SECTION *crit = &_crit;
		Thread *t = ThreadManager::GetSingleton().CreateNewThread( (void*)this, Bind( &Server::UserProcessHandler ), crit, (void*)newUser );
		newUser->SetThread( t );
		//User *u = GetUserByName( newUser->GetName() );
		//std::cout<<"New user: "<<newUser<<", found: "<<u<<"\n";
	}
	if( ++_delThreadCounter == 100 )
	{
		_delThreadCounter = 0;
		ThreadManager::GetSingleton().Cleanup();
	}
	return RET_SUCCESS;
}
int Server::HandleMainMsg( Message *const msg )
{
	//std::cout<<"Handling Main Message: "<<msg->GetName()<<"\n";
	switch( msg->ID )
	{
	case MSG_SEND_MASTER_LIST:
		{
			int c = 0;
			User *u = static_cast< User* >( msg->Obj );
			::EnterCriticalSection( &_crit );
			u->Counter = _users.size() - 1;
			::LeaveCriticalSection( &_crit );
			//
			Message *m;
			for( UserMap::iterator it1 = _users.begin(), it2 = _users.end(); it1 != it2; ++it1 )
			{
				if( it1->second != u )
				{
					++c;
					m = new Message( MSG_SEND_YOUR_LIST, u );
					it1->second->GetThread()->Messages()->Push( m );
				}
			}
			if( c == 0 )
			{
				m = new Message( MSG_NO_FILES_TO_DOWNLOAD, 0 );
				u->GetThread()->Messages()->Push( m );
			}
		}
		break;
	case MSG_SENT_MY_LIST:
		{
			//std::cout<<"One guy sent his list\n";
			User *u = static_cast< User* >( msg->Obj );
			int i;
			::EnterCriticalSection( &_crit );
			i = --u->Counter;
			::LeaveCriticalSection( &_crit );
			if( i < 1 )
			{
				SendMsgToUser( u, CMD_SENT_MASTER_LIST, u->GetThread() );
			}
		}
		break;
	case MSG_FILE_SHARE:
		{
			++_uniqueID;
			FileTransaction *t = static_cast< FileTransaction* >( msg->Obj );
			//std::cout<<"Uploader: "<<t->Uploader.c_str()<<", Downloader: "<<t->Downloader.c_str()<<", File: "<<t->FileName.c_str()<<"\n";
			User *uploader		= GetUserByName( t->Uploader );
			User *downloader	= GetUserByName( t->Downloader );
			if( uploader != 0 )
			{
				//std::cout<<"Uploader: "<<uploader->GetName().c_str()<<", Downloader: "<<downloader->GetName().c_str()<<"\n";
				FileTransaction *t1 = new FileTransaction( *t );
				t1->ID = _uniqueID;
				t1->Port.assign( downloader->UDPPort );
				Message *m1 = new Message( MSG_UPLOAD_FILE, t1 );
				uploader->GetThread()->Messages()->Push( m1 );
				//
				FileTransaction *t2 = new FileTransaction( *t );
				t2->ID = _uniqueID;
				t2->Port.assign( uploader->UDPPort );
				Message *m2 = new Message( MSG_DOWNLOAD_FILE, t2 );
				downloader->GetThread()->Messages()->Push( m2 );
			}
			else if( downloader != 0 )
			{
				Message *m2 = new Message( MSG_NO_LONGER_AVAILABLE, 0 );
				downloader->GetThread()->Messages()->Push( m2 );
			}
			//
			delete t;
		}
		break;
	default:
		{
			std::cout<<"Unhandled main Msg: "<<msg->GetName()<<"\n";
		}
	}
	return RET_SUCCESS;
}
int Server::UserProcessHandler( void *data, Thread *thread )
{
	User *user = ( User* )data;
	//std::cout<<"User Process Name: "<<user->GetName().c_str()<<"\n";
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
				HandleProcessMsg( msg, user );
			}
			delete msg;
		}
		else
		{
			switch( user->Status )
			{
			case ST_Invalid:
				{

				}
				break;
			case ST_NewlyConnected:
				{
					GetUserPort( user );
					user->Status = ST_ShareList;
				}
				break;
			case ST_ShareList:
				{
					GetUserFileList( user );
					user->Status = ST_WaitingForCommand;
				}
				break;
			case ST_WaitingForCommand:
				{
					std::string str1;
					int ret;
					if( (ret = ReceiveUserMsg(user, str1, thread)) == RET_SUCCESS )
					{
						std::vector< std::string > udata;
						utility::SplitString( str1, DELIMITER[0], udata );
						std::string *cmd = &udata.at( 0 );
						//
						if( !cmd->compare(CMD_MASTER_LIST) )
						{
							//std::cout<<"User wants master list\n";
							Message *m = new Message( MSG_SEND_MASTER_LIST, user );
							_msgs->Push( m );
						}
						else if( !cmd->compare(CMD_RQST_TO_DOWNLOAD_FILE) )
						{
							//std::cout<<"User wants to download: "<<udata.at( 1 ).c_str()<<": "<<udata.at( 2 ).c_str()<<"\n";
							FileTransaction *t	= new FileTransaction();
							t->Downloader		= user->GetName();
							t->Uploader			= udata.at( 1 );
							t->FileName			= udata.at( 2 );
							t->ID				= -1;
							t->Size				= udata.at( 3 );
							//
							Message *m = new Message( MSG_FILE_SHARE, t );
							_msgs->Push( m );
						}
					}
					else if( ret == RET_USER_QUIT || ret == RET_USER_QUIT_ABRUPTLY )
					{
						user->Status = ST_Quit;
					}
				}
				break;
			case ST_Quit:
				{
					//std::cout<<"User: "<<user->GetName().c_str()<<" has quit, destroying user\n";
					TCPConnection::CloseSocket( user->Socket()->socket );
					RemoveUser( user );
					run = false;
				}
				break;
			default:
				{

				}
			}
		}
	}
	return RET_SUCCESS;
}
int Server::HandleProcessMsg( Message *const msg, User *user )
{
	//std::cout<<"Handling Process Msg: "<<msg->GetName()<<", User: "<<user->GetName().c_str()<<"\n";
	switch( msg->ID )
	{
	case MSG_SEND_YOUR_LIST:
		{
			//std::cout<<"Sending my list\n";
			User *u = static_cast< User* >( msg->Obj );
			std::string share;
			user->GetShareList( share );
			//std::cout<<"My List is: "<<share.c_str()<<"\n";
			//
			SendMsgToUser( u, share, u->GetThread() );
			//
			Message *m = new Message( MSG_SENT_MY_LIST, u );
			_msgs->Push( m );
		}
		break;
	case MSG_UPLOAD_FILE:
		{
			FileTransaction *t = static_cast< FileTransaction* >( msg->Obj );
			char id[ 8 ];
			::_itoa_s( t->ID, id, 8, 10 );
			//
			std::string str( CMD_UPLOAD_FILE );
			str.append( DELIMITER );
			str.append( id );
			str.append( DELIMITER );
			str.append( t->Downloader );
			str.append( DELIMITER );
			str.append( t->Port );
			str.append( DELIMITER );
			str.append( t->FileName );
			str.append( DELIMITER );
			str.append( t->Size );
			//
			delete t;
			SendMsgToUser( user, str, user->GetThread() );
		}
		break;
	case MSG_DOWNLOAD_FILE:
		{
			FileTransaction *t = static_cast< FileTransaction* >( msg->Obj );
			char id[ 8 ];
			::_itoa_s( t->ID, id, 8, 10 );
			std::string str( CMD_DOWNLOAD_FILE );
			str.append( DELIMITER );
			str.append( id );
			str.append( DELIMITER );
			str.append( t->Uploader );
			str.append( DELIMITER );
			str.append( t->Port );
			str.append( DELIMITER );
			str.append( t->FileName );
			str.append( DELIMITER );
			str.append( t->Size );
			//
			delete t;
			SendMsgToUser( user, str, user->GetThread() );
		}
		break;
	case MSG_NO_LONGER_AVAILABLE:
		{
			std::string str( CMD_NO_LONGER_AVAILABLE );
			SendMsgToUser( user, str, user->GetThread() );
		}
		break;
	case MSG_NO_FILES_TO_DOWNLOAD:
		{
			std::string str( CMD_NO_FILES_TO_DOWNLOAD );
			SendMsgToUser( user, str, user->GetThread() );
		}
		break;
	default:
		{
			std::cout<<"Unhandled user msg: "<<msg->GetName()<<"\n";
		}
	}
	return RET_SUCCESS;
}
int Server::GetUserPort( User *user )
{
	std::string str;
	SendMsgToUser( user, CMD_PORT_ID, user->GetThread() );
	if( ReceiveUserMsgBlocking( user, str, user->GetThread() ) == RET_SUCCESS )
	{
		//std::cout<<"User sent: "<<str.c_str()<<"\n";
		//return user->PopulateShareList( str );
		std::vector< std::string > vec;
		utility::SplitString( str, DELIMITER[0], vec );
		if( !vec.at(0).compare(CMD_PORT_ID) )
		{
			user->UDPPort = vec.at( 1 );
		}
		else
		{
			return RET_FAIL;
		}
	}
	else
	{
		user->Status = ST_Quit;
	}
	return RET_SUCCESS;
}
int Server::GetUserFileList( User *user )
{
	std::string str;
	SendMsgToUser( user, CMD_SHARE_LIST, user->GetThread() );
	if( ReceiveUserMsgBlocking( user, str, user->GetThread() ) == RET_SUCCESS )
	{
		//std::cout<<"User sent: "<<str.c_str()<<"\n";
		return user->PopulateShareList( str );
	}
	else
	{
		user->Status = ST_Quit;
	}
	return RET_SUCCESS;
}
User* Server::GetUserByName( const std::string &name )
{
	User *u = 0;
	UserMap::iterator it = _users.find( name );
	if( it != _users.end() )
	{
		u = it->second;
	}
	//
	return u;
}
int Server::SendMsgToUser( User *&user, const char *str, Thread *thread )
{
	return SendMsgToUser( user, std::string(str), thread );
}
int Server::SendMsgToUser( User *&user, const std::string &str, Thread *thread )
{
	SOCKET plSocket = user->Socket()->socket;
	int start = 0, len = 0, ret;
	const int BUFF_LEN = 256;
	char buffer[ BUFF_LEN ];
	//
	// code to send typed data as 255 byte chunks
	do
	{
		len = str.copy( buffer, BUFF_LEN-1, start );
		buffer[len] = '\0';
		start += len;
		//
		//std::cout<<"Sending: "<<buffer<<"\n";
		ret = TCPConnection::Send( plSocket, buffer, ++len );
	}
	while( len == BUFF_LEN );
	//
	return RET_SUCCESS;
}
int Server::ReceiveUserMsgBlocking( User *&user, std::string &str, Thread *thread )
{
	int ret, spin = 0;
	const int SPIN_COUNT = 20;
	do
	{
		ret = ReceiveUserMsg( user, str, thread );
		if( ++spin > SPIN_COUNT )
		{
			spin = 0;
			Sleep( 1 );
			continue;
		}
	}
	while( ret == RET_NO_SOCKET_DATA );
	//
	return ret;
}
int Server::ReceiveUserMsg( User *&user, std::string &str, Thread *thread )
{
	int ret;
	const int BUFF_LEN = 256;
	char buffer[ BUFF_LEN ];
	SOCKET plSocket = user->Socket()->socket;
	ret = TCPConnection::Receive( plSocket, buffer, BUFF_LEN );
	if( TCPConnection::WouldBlock(ret) )
	{
		return RET_NO_SOCKET_DATA;
	}
	else if( TCPConnection::IsConnReset(ret) )
	{
		thread->StartSync();
		std::cout<<user->Socket()->address<<": Closed Abruptly\n";
		thread->EndSync();
		//
		return RET_USER_QUIT_ABRUPTLY;
	}
	else if( ret == 0 )
	{
		thread->StartSync();
		std::cout<<user->Socket()->address<<": Closed Gracefully\n";
		thread->EndSync();
		//
		return RET_USER_QUIT;
	}
	else
	{
		str.assign( buffer, ret - 1 );
		thread->StartSync();
		//std::cout <<user->Socket()->address<<": "<<str<<"\n";
		thread->EndSync();
		//
		return RET_SUCCESS;
	}
}
int Server::RemoveUser( User *&user )
{
	for( UserMap::iterator it1 = _users.begin(), it2 = _users.end(); it1 != it2; ++it1 )
	{
		if( it1->second == user )
		{
			_users.erase( it1 );
			break;
		}
	}
	_TCPConn->RemoveClient( const_cast<ClientSocket*>(user->Socket()) );
	delete user;
	//
	return RET_SUCCESS;
}
int Server::ReadConfigFile( const char *config, ConfigData *data )
{
	FILE *file;
	const int BuffLen = 100;
	char line[ BuffLen ];
	//
	if ( ::fopen_s(&file, config, "r") != 0 )
	{
		std::cout << "[ERROR: Server: 1]: "<<config<<" could not be opened for reading\n" << "\n";
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