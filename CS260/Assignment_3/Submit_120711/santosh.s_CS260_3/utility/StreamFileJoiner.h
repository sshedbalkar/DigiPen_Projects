#pragma once
//
#include "StreamFileSplitter.h"
#include <list>
//
struct FileChunk;
//
#define MAX_IN_MEM_SIZE		64
//
class StreamFileJoiner
{
public:
							StreamFileJoiner();
	virtual					~StreamFileJoiner();
	//
public:
	int						Init( const char *file, long long &size );
	int						AddChunk( FileChunk &chunk );
	float					Flush( void );
	long long				TotalSize( void );
	long long				CurrentTotalSize( void );
	bool					IsDone( void );
	//
private:
	char					_buffer[ BUFFER_SIZE ];
	FILE					*_file;
	unsigned int			_nextID;
	std::list< FileChunk >	_chunks;
	long long				_size;
	size_t					_thresholdSize;
	long long				_joined;
	size_t					_listSize;
	size_t					_itemSize;
	int						_lastIDWritten;
};