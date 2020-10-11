#pragma once
//
#include <string>
#include <vector>
//
struct	ClientSocket;
class	Thread;
//
typedef enum
{
	ST_Invalid,
	ST_NewlyConnected,
	ST_ShareList,
	ST_WaitingForCommand,
	ST_Quit
} UserStatus;
//
class User
{
public:
						User( ClientSocket *&sock );
	virtual				~User();
	//
public:
	typedef				std::pair< std::string, std::string >	FileSizePair;
	typedef				std::vector< FileSizePair >				UserShareList;
	ClientSocket*		Socket( void );
	const std::string&	GetName( void );
	static std::string	CreateName( ClientSocket *&sock );
	void				SetThread( Thread *t );
	Thread*				GetThread( void );
	int					PopulateShareList( const std::string &str );
	int					GetShareList( std::string &share );
	//
public:
	UserStatus			Status;
	int					Counter;
	std::string			UDPPort;
	//
private:
	//
private:
	ClientSocket		*_clientSocket;
	std::string			_name;
	Thread				*_thread;
	UserShareList		_shareList;
};