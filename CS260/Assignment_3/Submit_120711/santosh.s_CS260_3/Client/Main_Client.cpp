#include "../utility/FileData.h"
#include <iostream>
#include "../os/WindowsIncludes.h"
#include <vector>
#include <string>
#include "../common/ReturnCodes.h"
#include "../debugTools/DebugTools.h"
#include "../debugTools/OutputConsole.h"
#include "../utility/Utility.h"
#include "client\Client.h"
#include "../threading/ThreadManager.h"
//
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	utility::Init();
	debug::DebugTools d;
	debug::DebugTools::getInstance()->enable( true );
	debug::OutputConsole::CreateConsole();
	//
	//
	ThreadManager *tm = new ThreadManager();
	//
	std::cout<<"Program: "<<__argv[0]<<"\n";
	char *config = "../config.txt";
	if( __argc > 1 )
	{
		config = __argv[1];
	}
	//std::cout<<"Config: "<<config<<"\n";
	Client *c = new Client();
	if( c->Init( config ) == RET_SUCCESS )
	{
		while( c->Run() == RET_SUCCESS )
		{

		}
	}
	//
	delete c;
	delete tm;
	//std::cin.get();
	//
	return 0;
}