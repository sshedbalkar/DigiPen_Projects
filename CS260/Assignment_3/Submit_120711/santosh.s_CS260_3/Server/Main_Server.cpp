#include "../os/WindowsIncludes.h"
#include <iostream>
#include "../common/ReturnCodes.h"
#include "../debugTools/DebugTools.h"
#include "../debugTools/OutputConsole.h"
#include "../utility/Utility.h"
#include "server\Server.h"
#include "../threading/ThreadManager.h"
//
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	utility::Init();
	debug::DebugTools d;
	debug::DebugTools::getInstance()->enable( true );
	debug::OutputConsole::CreateConsole();
	//
	ThreadManager *tm = new ThreadManager();
	//
	std::cout<<"Program: "<<__argv[0]<<"\n";
	char *config = "../config.txt";
	if( __argc > 1 )
	{
		config = __argv[1];
	}
	//
	Server *s = new Server();
	if( s->Init( config ) == RET_SUCCESS )
	{
		while( s->Run() == RET_SUCCESS )
		{

		}
	}
	//
	delete s;
	delete tm;
	//
	//std::cin.get();
	//
	return 0;
}