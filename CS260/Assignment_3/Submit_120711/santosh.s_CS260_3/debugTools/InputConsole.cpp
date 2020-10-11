#include "InputConsole.h"
#include "../threading/ThreadManager.h"
#include "../common/ReturnCodes.h"
#include "Assert.h"
#include "../threading/Thread.h"
#include "../common/ReturnCodes.h"
#include "../message/Message.h"
#include "../threading/MsgQueue.h"
#include "../utility/Utility.h"
//
namespace debug
{
	InputConsole::InputConsole()
		:_thread( 0 ),
		_crit( 0 ),
		_ipHandle( 0 ),
		_opHandle( 0 )
	{
		//
	}
	//
	InputConsole::~InputConsole()
	{
		MCR_Assert( _thread != 0, "Input Thread is null");
		ThreadManager::GetSingleton().DeleteThread( _thread );
		//
		MCR_Assert( _crit != 0, "Critical section is null");
		::DeleteCriticalSection( _crit );
		delete _crit;
		//
		MCR_Assert( (_ipHandle != 0) && (_opHandle != 0), "IP/OP Handle is 0\n" );
		::CloseHandle( _ipHandle );
		::CloseHandle( _opHandle );
		//
		for( FunctionMap::iterator it1 = _funcMap.begin(), it2 = _funcMap.end(); it1 != it2; ++it1 )
		{
			delete &( *it1 ).second.second;
		}
		//
		std::cout<<"InputConsole Dtr\n";
	}
	int InputConsole::Init()
	{
		_crit = new CRITICAL_SECTION();
		::InitializeCriticalSection( _crit );
		//
		_ipHandle = ::GetStdHandle( STD_INPUT_HANDLE );
		_opHandle = ::GetStdHandle( STD_OUTPUT_HANDLE );
		::GetConsoleScreenBufferInfo( _opHandle, &_csbInfo );
		//
		_thread = ThreadManager::GetSingleton().CreateNewThread( (void*)this, Bind( &InputConsole::InputThread ), _crit, 0 );
		//
		return RET_SUCCESS;
	}
	int InputConsole::RegisterCommand( const char *command, Object obj, FunctionObject *&func )
	{
		std::string comm( command );
		return RegisterCommand( comm, obj, func );
	}
	int InputConsole::RegisterCommand( std::string &command, Object obj, FunctionObject *&func )
	{
		utility::ToLower( command );
		if( _funcMap.find(command) == _funcMap.end() )
		{
			//std::cout<<"Adding command: "<<command.c_str()<<"\n";
			_funcMap.insert( CommandPair(command, FuncPair(obj, func)) );
			return RET_SUCCESS;
		}
		//
		return RET_ALREADY_EXISTS;
	}
	int InputConsole::UnregisterCommand( const char *command )
	{
		std::string comm( command );
		return UnregisterCommand( comm );
	}
	int InputConsole::UnregisterCommand( std::string &command )
	{
		utility::ToLower( command );
		FunctionMap::iterator it1 = _funcMap.find(command);
		if( it1 != _funcMap.end() )
		{
			std::cout<<"Removing command: "<<command.c_str()<<"\n";
			delete ( *it1 ).second.second;
			_funcMap.erase( it1 );
			return RET_SUCCESS;
		}
		else
		{
			std::cout<<"Could not find command: "<<command.c_str()<<" to unregister\n";
			return RET_DOESNT_EXIST;
		}
	}
	int InputConsole::GetNextLine( std::string &str )
	{
		_thread->StartSync();
		if( !_ipBuffer.empty() )
		{
			str = _ipBuffer.front(); 
			_ipBuffer.pop();
		}
		_thread->EndSync();
		return str.size();
	}
	int InputConsole::ExecuteNextCommand( void )
	{
		std::string str;
		if( GetNextLine( str ) )
		{
			std::vector< std::string > vec;
			utility::SplitString( str, ' ', vec );
			if( !vec.empty() )
			{
				std::string comm = vec.front();
				vec.erase( vec.begin() );
				std::cout<<"Executing command: "<<comm.c_str()<<", Len: "<<vec.size()<<"\n";
				FunctionMap::iterator it1 = _funcMap.find(comm);
				if( it1 != _funcMap.end() )
				{
					ArgumentVector v1;
					v1.push_back( &vec );
					FuncPair *f = &( *it1 ).second;
					f->second->Invoke( f->first, v1 );
					return RET_SUCCESS;
				}
				else
				{
					std::cout<<"Command: "<<comm.c_str()<<" is not registered\n";
					return RET_DOESNT_EXIST;
				}
			}
		}
		//
		return RET_SUCCESS;
	}
	int InputConsole::InputThread( void *data, Thread *thread )
	{
		bool quit = false;
		//_thread->StartSync();
		//std::cout<<"Input thread started\n";
		//_thread->EndSync();
		do
		{
			DWORD result = ::WaitForSingleObject( _ipHandle, 10 );
			switch( result )
			{
				case WAIT_OBJECT_0:
				{
					HandleInput();
				}
				break;
			case WAIT_TIMEOUT:
				{
					if( !(_thread->Messages()->IsEmpty()) )
					{
						Message *msg = _thread->Messages()->Front();
						if( msg->ID == MSG_QUIT )
						{
							quit = true;
						}
						else
						{
							HandleMsg( msg );
						}
						delete msg;
					}
				}
				break;
			default:
				MCR_Assert( false, "Wait for input event failed" );
			}
		}
		while( !quit );
		//
		thread->StartSync();
		std::cout<<"Ending Input console thread\n";
		thread->EndSync();
		return RET_SUCCESS;
	}
	/*
	return = 1			User pressed y/Y
	return = 0			User pressed n/N
	*/
	int InputConsole::UserInputTrue( void )
	{
		std::string str;
		int ret;
		do
		{
			str.clear();
			while( !GetNextLine( str ) ){}
			utility::ToLower( str );
			if( !str.compare("y") )
			{
				ret = 1;
			}
			else if( !str.compare("n") )
			{
				ret = 0;
			}
			else
			{
				ret = -1;
			}
		}
		while( ret < 0 );
		//
		return ret;
	}
	int InputConsole::HandleInput( void )
	{
		std::string str;
		if( ReadLine(str) && (!str.empty()) )
		{
			//std::cout<<"[You]: "<<str.c_str()<<", Len: "<<str.size()<<"\n";
			//std::vector< std::string > vec;
			//utility::SplitString( str, ' ', vec );
			_thread->StartSync();
			_ipBuffer.push( str );
			_thread->EndSync();
		}
		return RET_SUCCESS;
	}
	int InputConsole::HandleMsg( Message *const msg )
	{
		_thread->StartSync();
		std::cout<<"Handling message: "<<msg->GetName()<<"\n";
		_thread->EndSync();
		//
		return RET_SUCCESS;
	}
	/*!
	Adds all pending keyboard input to the internal buffer.   The
	implementation uses ReadConsoleInput() which is a non-blocking function.
	All non-keyboard events are ignored.
	*/
	void InputConsole::AddPendingInputToBuffer( void )
	{
		//
		// Read pending events
		//
		DWORD dwRead;
		INPUT_RECORD inRecords[ 128 ];
		::ReadConsoleInput( _ipHandle, &inRecords[ 0 ], 128, &dwRead );

		//
		// Process the events
		//
		for( unsigned int i = 0; i < dwRead; ++i )
		{
			if( inRecords[ i ].EventType == KEY_EVENT )
			{
				// Key events include non-character keys like SHIFT.  Filter
				// these out.
				const KEY_EVENT_RECORD& evt = inRecords[ i ].Event.KeyEvent;
				const char c = evt.uChar.AsciiChar;
				if( evt.bKeyDown )
				{
					if( c != 0 )
					{
						// Add to the buffer
						_buffer.append( &c, 1 );
						//
						// Emulate the default behavior by echoing the typed characters
						std::cout << c;
						//
						// Unfortunately since keyboard input is being handled manually
						// behavior such as backspace needs to be manually emulated.
						if ( c == '\b' )
						{
							HANDLE hIp = ::GetStdHandle( STD_INPUT_HANDLE );
							if (hIp == INVALID_HANDLE_VALUE) 
							{
								//std::cout<<"Error handle\n";
							}
							CONSOLE_SCREEN_BUFFER_INFO info;
							DWORD dwWritten = 0;
							::GetConsoleScreenBufferInfo( hIp, &info );
							if( ::WriteConsoleOutputCharacter( hIp, " ", 1, info.dwCursorPosition, &dwWritten ) == 0 )
							{
								//std::cout<<"Wrote: "<<dwWritten<<": "<<hIp<<", x: "<<info.dwCursorPosition.X<<", y: "<<info.dwCursorPosition.Y<<", Err: "<<::GetLastError()<<"\n";
							}
						}
					}
				}
			}
		}
	}
	/*!
		Scan the buffer for a full line of input. Return true if there
		is a full line available.
	*/
	bool InputConsole::ReadNextLineFromBuffer( std::string& line )
	{
		size_t pos = _buffer.find( '\r' );
		if( pos != std::string::npos )
		{
			// Split the buffer at the first line break.  Set the return value
			// to all text prior to that line break and leave the remaining
			// characters in the buffer.
			//
			line = _buffer.substr( 0, pos );
			size_t pos2 = _buffer.size();
			pos2 -= pos;
			++pos2;
			_buffer = _buffer.substr( pos + 1, pos2 );
			return true;
		}
		else
		{
			// No line break found.
			return false;
		}
	}
	bool InputConsole::ReadLine( std::string& line )
	{
		AddPendingInputToBuffer();
		if( ReadNextLineFromBuffer( line ))
		{
			CONSOLE_SCREEN_BUFFER_INFO info;
			::GetConsoleScreenBufferInfo( _opHandle, &info );
			//
			info.dwCursorPosition.X = 0;
			/*
			std::cout<<"Max: "<<info.dwSize.Y<<", Curr: "<<info.dwCursorPosition.Y<<"\n";
			if ( (info.dwSize.Y - 1) == info.dwCursorPosition.Y )
			{
				ScrollScreenBuffer( _opHandle, 1 ); 
			}
			*/
			++info.dwCursorPosition.Y;
			//
			::SetConsoleCursorPosition( _opHandle, info.dwCursorPosition );
			::SetConsoleCursorPosition( _ipHandle, info.dwCursorPosition );
			//printf("\r");
			//std::cout<<"\r\n";
			return true;
		}
		return false;
	}
	int InputConsole::ScrollScreenBuffer( HANDLE h, const int &x )
	{
		SMALL_RECT srctScrollRect, srctClipRect;
		CHAR_INFO chiFill;
		COORD coordDest;

		srctScrollRect.Left = 0;
		srctScrollRect.Top = 1;
		srctScrollRect.Right = _csbInfo.dwSize.X - (SHORT)x; 
		srctScrollRect.Bottom = _csbInfo.dwSize.Y - (SHORT)x; 
 
		// The destination for the scroll rectangle is one row up. 

		coordDest.X = 0; 
		coordDest.Y = 0; 
 
		// The clipping rectangle is the same as the scrolling rectangle. 
		// The destination row is left unchanged. 
 
		srctClipRect = srctScrollRect; 
 
		// Set the fill character and attributes. 
 
		chiFill.Attributes = FOREGROUND_RED|FOREGROUND_INTENSITY; 
		chiFill.Char.AsciiChar = (char)' '; 
 
		// Scroll up one line. 
 
		ScrollConsoleScreenBuffer( 
			h,               // screen buffer handle 
			&srctScrollRect, // scrolling rectangle 
			&srctClipRect,   // clipping rectangle 
			coordDest,       // top left destination cell 
			&chiFill);       // fill character and color 
		//
		return RET_SUCCESS;
	}
}