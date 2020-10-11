#pragma once
//
#include "../os/WindowsIncludes.h"
#include <vector>
#include <string>
//
namespace utility
{
	struct FileInfo
	{
		FileInfo();
		FileInfo( const char* name, long long &size );
		FileInfo( const char* name, long long &size, const char *fullName );
		~FileInfo();
		//
		std::string			Name;
		long long			Size;
		std::string			FullName;
	};
	typedef std::vector< FileInfo > FileInfoList;
	//
	int					ListFilesInDirectory( const char *directoryName, FileInfoList &filenameList );
	int					ReadNextLine( char *buff, const int &buffLen, FILE *hFile );
	char*				ReadFile( const char* file );
	bool				CanOpenFile( const char* file );
}