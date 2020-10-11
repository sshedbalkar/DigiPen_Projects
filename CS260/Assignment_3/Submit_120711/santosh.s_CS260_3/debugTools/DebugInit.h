#pragma once
//
#ifdef _DEBUG
	#define ENABLE_DEBUG_DIAGNOSTICS 1
#endif
//
#include "../os/WindowsIncludes.h"
//
//
void FDebugPrintHandler( const char *msg, ... )
{	
	const int BufferSize = 1024;
	char FinalMessage[BufferSize];
	va_list args;
	va_start(args, msg);
	vsnprintf_s(FinalMessage, BufferSize, _TRUNCATE, msg, args);
	va_end(args);

	OutputDebugString(FinalMessage);
	OutputDebugString("\n");
}

//A basic error output function
bool FSignalErrorHandler( const char *exp, const char *file, int line, const char *msg = 0, ... )
{
	const int BufferSize = 1024;
	char FinalMessage[BufferSize];

	//Print out the file and line in visual studio format so the error can be
	//double clicked in the output window file(line) : error
	int offset = sprintf_s( FinalMessage,"%s(%d) : ", file, line );
	if (msg != NULL)
	{	
		va_list args;
		va_start(args, msg);
		vsnprintf_s(FinalMessage + offset, BufferSize - offset, _TRUNCATE , msg, args);
		va_end(args);
	}
	else
	{
		strcpy_s(FinalMessage + offset, BufferSize - offset, "No Error Message");
	}

	//Print to visual studio output window
	OutputDebugString(FinalMessage);
	OutputDebugString("\n");

	//Display a message box to the user
	MessageBoxA(NULL, FinalMessage, "Error", 0);
	
	//Do not debug break
	return true;
}
//
namespace debug
{
	void init()
	{
		debug::SetErrorHandler( &FSignalErrorHandler );
		debug::SetDebugPrintHandler( &FDebugPrintHandler );
	}
}
