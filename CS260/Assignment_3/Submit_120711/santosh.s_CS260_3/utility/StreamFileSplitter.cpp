#include "StreamFileSplitter.h"
#include "../common/ReturnCodes.h"
#include <iostream>
#include "../debugTools/Assert.h"
//
/////////////////////////////////////////////////////////////////
FileChunk::FileChunk()
	:ID( -1 ),
	Payload( 0 ),
	Size( 0 )
{
	//
}
FileChunk::~FileChunk()
{
	//
}
/////////////////////////////////////////////////////////////////
StreamFileSplitter::StreamFileSplitter()
	:_file( 0 ),
	_nextID( 0 ),
	_itemSize( sizeof(char) )
{
	//
}
StreamFileSplitter::~StreamFileSplitter()
{
	MCR_Assert( _file != 0, "File Pointer is NULL" );
	::fclose( _file );
}
int StreamFileSplitter::Init( const char* file, size_t chunkSize )
{
	if( ::fopen_s(&_file, file, "rb") )
	{
		std::cout<<"[ERR: StreamFileSplitter: 1]: "<<file<<"\n";
		return RET_BAD_SOURCE;
	}
	_chunkSize = chunkSize;
	//
	return RET_SUCCESS;
}
int StreamFileSplitter::ReadNextChunk( FileChunk *chunk )
{
	size_t items_left_to_read = _chunkSize;
	size_t items_to_read = (items_left_to_read > BUFFER_SIZE) ? BUFFER_SIZE : items_left_to_read;
	size_t item_count = 0, items_read;
	char *target = 0;
	size_t pos = 0;
	//
	do
	{
		items_read = ::fread( _buffer, _itemSize, items_to_read/_itemSize, _file );
		if( ::ferror(_file) )
		{
			std::cout<<"[ERR: StreamFileSplitter: 2]: fread error\n";
			break;
		}
		if( items_read > 0 )
		{
			if( target == 0 )
			{
				target = new char[ _chunkSize + 1 ];
			}
			items_left_to_read -= items_read;
			for( size_t i = 0; i < items_read; ++i )
			{
				target[ pos ] = _buffer[ i ];
				++pos;
			}
			item_count += items_read;
		}
	}
	while( (items_left_to_read > 0) && !::feof(_file) );
	//
	//std::cout<<"Pos: "<<pos<<"\n";
	//
	if( item_count > 0 )
	{
		target[ pos ] = '\0';
		//
		chunk->ID		= _nextID;
		chunk->Payload	= target;
		++_nextID;
		chunk->Size = item_count * _itemSize;
		//std::cout<<chunk->ID<<": "<<chunk->Size<<": "<<chunk->Payload<<"\n";
		return RET_SUCCESS;
	}
	return RET_NO_MORE_DATA;
}
unsigned int StreamFileSplitter::NextID( void )
{
	return _nextID;
}
int StreamFileSplitter::ReadRandomChunk( FileChunk *chunk, const unsigned int &chunkID )
{
	if( _nextID != chunkID )
	{
		size_t offset = chunkID * _chunkSize;
		int result = ::fseek( _file, offset, SEEK_SET);
		if( result )
		{
			std::cout<<"[ERR: StreamFileSplitter: 3]: fseek failed\n";
			return RET_FAIL;
		}
		_nextID = chunkID;
	}
	return ReadNextChunk( chunk );
}
int StreamFileSplitter::ReadAll( FileChunkList &list )
{
	FileChunk chunk;
	ReadRandomChunk( &chunk, 0 );
	list.push_back( chunk );
	while( ReadNextChunk( &chunk ) == RET_SUCCESS )
	{
		list.push_back( chunk );
	}
	return RET_SUCCESS;
}