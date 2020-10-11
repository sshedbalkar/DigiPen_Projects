#include "User.h"
#include "../../networking/TCPConnection.h"
#include "../../utility/Utility.h"
#include "../../Client/client/CommandCodes.h"
#include "../../common/ReturnCodes.h"
#include <iostream>
//
User::User( ClientSocket *&sock )
	:_clientSocket( sock ),
	_name( CreateName(sock) ),
	Status( ST_Invalid ),
	_thread( 0 )
{
	//
}
User::~User()
{
	//
}
ClientSocket* User::Socket( void )
{
	return _clientSocket;
}
const std::string& User::GetName( void )
{
	return _name;
}
void User::SetThread( Thread *t )
{
	_thread = t;
}
Thread* User::GetThread( void )
{
	return _thread;
}
int User::PopulateShareList( const std::string &str )
{
	std::vector< std::string > vec;
	utility::SplitString( str, DELIMITER[0], vec );
	if( !vec.at(0).compare(CMD_SHARE_LIST) )
	{
		int total = ::atoi( vec.at(1).c_str() );
		for( std::vector< std::string >::iterator it1 = vec.begin() + 2, it2 = vec.end(), it3; it1 != it2; it1 += 2 )
		{
			//std::cout<<"Pushing: "<<(*it1).c_str()<<"\n";
			it3 = it1 + 1;
			_shareList.push_back( FileSizePair((*it1), (*it3)) );
		}
		//
		//std::cout<<"Total files: "<<_shareList.size()<<"\n";
		return RET_SUCCESS;
	}
	return RET_FAIL;
}
int User::GetShareList( std::string &share )
{
	share.assign( CMD_A_SHARE_LIST );
	share.append( DELIMITER );
	//
	//share.append( inet_ntoa(_clientSocket->address.sin_addr) );
	share.append( _name );
	share.append( DELIMITER );
	//
	char total[ 4 ];
	::_itoa_s( _shareList.size(), total, 4, 10 );
	share.append( total );
	//
	for each( FileSizePair str in _shareList )
	{
		share.append( DELIMITER );
		share.append( str.first );
		share.append( DELIMITER );
		share.append( str.second );
	}
	//
	return RET_SUCCESS;
}
std::string User::CreateName( ClientSocket *&sock )
{
	std::string name;
	char port[ 7 ];
	sockaddr_in *addr = &sock->address;
	name.assign( inet_ntoa(addr->sin_addr) );
	name.append( ":" );
	::_itoa_s( (int)ntohs(addr->sin_port), port, 7, 10 );
	name.append( port );
	//
	return name;
}