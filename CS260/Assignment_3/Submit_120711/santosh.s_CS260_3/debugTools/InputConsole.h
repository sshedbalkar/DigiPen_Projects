#pragma once
//
#include <queue>
#include "../common/FunctionBind.h"
#include "../common/Singleton.h"
#include "../os/WindowsIncludes.h"
#include <string>
#include <vector>
#include <hash_map>
//
class Thread;
class Message;
//
namespace debug
{
	class InputConsole: public Singleton< InputConsole >
	{
	public:
									InputConsole();
		virtual						~InputConsole();
		//
		int							Init();
		//
	public:
		int							HandleInput( void );
		int							RegisterCommand( std::string &command, Object obj, FunctionObject *&func );
		int							RegisterCommand( const char *command, Object obj, FunctionObject *&func );
		int							UnregisterCommand( std::string &command );
		int							UnregisterCommand( const char *command );
		int							ExecuteNextCommand( void );
		int							GetNextLine( std::string &str );
		int							UserInputTrue( void );
		//
	private:
		int							InputThread( void *data, Thread *thread );
		int							HandleMsg( Message *const msg );
		void						AddPendingInputToBuffer( void );
		bool						ReadNextLineFromBuffer( std::string& line );
		bool						ReadLine( std::string& line );
		int							ScrollScreenBuffer( HANDLE h, const int &x );
		//
	private:
		typedef std::queue< std::string >							InputBuffer;
		typedef std::pair< Object, FunctionObject* >				FuncPair;
		typedef std::pair< std::string, FuncPair >					CommandPair;
		typedef stdext::hash_map< std::string, FuncPair >			FunctionMap;
		
		//
		InputBuffer					_ipBuffer;
		FunctionMap					_funcMap;
		Thread						*_thread;
		CRITICAL_SECTION			*_crit;
		std::string					_buffer;
		HANDLE						_ipHandle;
		HANDLE						_opHandle;
		CONSOLE_SCREEN_BUFFER_INFO	_csbInfo;
	};
}