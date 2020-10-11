#pragma once
//
#include <stdio.h>
#include <vector>
//
struct FileChunk
{
						FileChunk();
	virtual				~FileChunk();
	//
	int					ID;
	char				*Payload;
	size_t				Size;
};
////////////////////////////////////////////////////////////////////////////
#define BUFFER_SIZE		5120
//
typedef std::vector< FileChunk > FileChunkList;
//
class StreamFileSplitter
{
public:
					StreamFileSplitter();
	virtual			~StreamFileSplitter();
	//
public:
	int				Init( const char* file, size_t chunkSize = 1024 );
	int				ReadNextChunk( FileChunk *chunk );
	int				ReadRandomChunk( FileChunk *chunk, const unsigned int &chunkID );
	unsigned int	NextID( void );
	int				ReadAll( FileChunkList &list );
	//
private:
	//
private:
	char			_buffer[ BUFFER_SIZE ];
	FILE			*_file;
	unsigned int	_nextID;
	size_t			_chunkSize;
	size_t			_itemSize;
};