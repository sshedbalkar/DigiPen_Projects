#pragma once
//
#include "../../os/WindowsIncludes.h"
#include <vector>
#include <string>
#include "../../utility/FileData.h"
#include <hash_map>
//
#define CONNECTION_CONFIG_FILE	"../config.txt"
#define BUFF_LEN				256
#define UDP_BUFF_LEN			5376
#define CHUNK_SIZE				5120
//
struct		ConfigData;
class		TCPConnection;
class		UDPConnection;
class		Thread;
class		StreamFileJoiner;
class		MsgQueue;
class		Message;
namespace	debug
{
	class	InputConsole;
}
//
struct DownloadData
{
	StreamFileJoiner	*Joiner;
	std::string			Name;
};
//
class Client
{
public:
								Client();
	virtual						~Client();
	//
public:
	int							Init( const char *configFile );
	int							Run( void );
	//
private:
	typedef std::pair< std::string, std::string >			FileSizePair;
	typedef std::pair< std::string, FileSizePair >			FileIPPair;
	typedef std::pair< std::string, DownloadData >			DownloadPair;
	typedef stdext::hash_map< std::string, DownloadData >	DownloadMap;
	//
	int							ReadConfigFile( const char *config, ConfigData *data );
	int							CreateSendFileList( const char *dir );
	int							ReadServerMsg( std::string &str );
	int							ReadServerMsgBlocking( std::string &str );
	int							SendServerMsg( const std::string &str );
	int							SendServerMsg( const char *str );
	int							SendShareList( void );
	int							SendPortID( void );
	int							GetMasterShareList( void );
	int							AddToMasterList( std::vector< std::string > &alist );
	int							ShowMasterList( void );
	int							RequestToDownloadFile( FileIPPair &pair );
	int							DownloadFile( const std::string &uploader, const std::string &file, const std::string &id, const std::string &size, const std::string &uport );
	int							UploadFile( const std::string &downloader, const std::string &file, const std::string &id, const std::string &size, const std::string &uport );
	int							GetFullFileName( const std::string &file, std::string &full );
	int							UploaderThread( void *data, Thread *thread );
	int							DownloaderThread( void *data, Thread *thread );
	int							SendUDPMsg( const std::string &str, sockaddr_in &addr );
	int							SendUDPMsg( const char *str, sockaddr_in &addr );
	int							HandleDownloaderMsg( Message *msg );
	//
private:
	typedef enum 
	{
		ST_Invalid = 0,
		ST_Error,
		ST_Connecting,
		ST_NewlyConnected,
		ST_WaitingForUser,
		ST_MasterList,
		ST_Menu
	} ClientStatus;
	//
	std::vector< FileIPPair >	_masterList;
	utility::FileInfoList		_sendList;
	TCPConnection				*_TCPConn;
	UDPConnection				*_UDPConn;
	debug::InputConsole			*_input;
	ClientStatus				_status;
	char						_buffer[ BUFF_LEN ];
	char						_UDPSendBuff[ BUFF_LEN ];
	char						_UDPRecvBuff[ UDP_BUFF_LEN ];
	CRITICAL_SECTION			_crit;
	int							_delThreadCounter;
	DownloadMap					_downloadMap;
	MsgQueue					*_msgs;
	Thread						*_downloadThread;
	int							_activeDls;
	int							_activeUps;
	ConfigData					*_config;
};