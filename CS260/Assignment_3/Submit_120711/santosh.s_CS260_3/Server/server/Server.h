#pragma once
//
#include "../../os/WindowsIncludes.h"
#include <hash_map>
#include <string>
#include "User.h"
//
#define CONNECTION_CONFIG_FILE	"../config.txt"
//
struct	ConfigData;
class	TCPConnection;
class	Message;
class	MsgQueue;
class	Message;
namespace debug
{
	class InputConsole;
}
//
class Server
{
public:
								Server();
	virtual						~Server();
	//
public:
	int							Init( const char *configFile );
	int							Run( void );
	//
private:
	int							ReadConfigFile( const char *config, ConfigData *data );
	User*						GetUserByName( const std::string &name );
	int							UserProcessHandler( void *data, Thread *thread );
	int							HandleProcessMsg( Message *const msg, User *user );
	int							GetUserFileList( User *user );
	int							GetUserPort( User *user );
	int							ReceiveUserMsg( User *&user, std::string &str, Thread *thread );
	int							ReceiveUserMsgBlocking( User *&user, std::string &str, Thread *thread );
	int							SendMsgToUser( User *&user, const std::string &str, Thread *thread );
	int							SendMsgToUser( User *&user, const char *str, Thread *thread );
	int							HandleMainMsg( Message *const msg );
	int							RemoveUser( User *&user );
	//
private:
	typedef						stdext::hash_map< std::string, User* >	UserMap;
	typedef						std::pair< std::string, User* >			UserIDPair;
	TCPConnection				*_TCPConn;
	debug::InputConsole			*_input;
	CRITICAL_SECTION			_crit;
	MsgQueue					*_msgs;
	UserMap						_users;
	int							_delThreadCounter;
	int							_uniqueID;
};