#include "StreamFileJoiner.h"
#include <iostream>
#include "../common/ReturnCodes.h"
#include "../debugTools/Assert.h"
//
bool CompareChunks( FileChunk &first, FileChunk &second )
{
	return first.ID < second.ID;
}
//
StreamFileJoiner::StreamFileJoiner()
	:_file( 0 ),
	_nextID( 0 ),
	_joined( 0L ),
	_thresholdSize( MAX_IN_MEM_SIZE ),
	_listSize( 0 ),
	_itemSize( sizeof(char) ),
	_lastIDWritten( -1 )
{
	//
}
StreamFileJoiner::~StreamFileJoiner()
{
	if( _file != 0 )
	{
		::fclose( _file );
		_file = 0;
	}
}
int StreamFileJoiner::Init( const char *file, long long &size )
{
	if( ::fopen_s(&_file, file, "wb") )
	{
		std::cout<<"[ERR: StreamFileJoiner: 1]: "<<file<<"\n";
		return RET_BAD_SOURCE;
	}
	_size = size;
	//
	return RET_SUCCESS;
}
int StreamFileJoiner::AddChunk( FileChunk &chunk )
{
	bool found = false;
	for each( FileChunk ch in _chunks )
	{
		if( ch.ID == chunk.ID )
		{
			delete chunk.Payload;
			return RET_SUCCESS;
		}
	}
	if( _lastIDWritten < chunk.ID )
	{
		_chunks.push_back( chunk );
		_chunks.sort( CompareChunks );
		_listSize += chunk.Size;
		if( (_listSize > _thresholdSize) || (_joined + _listSize >= _size) )
		{
			Flush();
		}
	}
	else
	{
		delete chunk.Payload;
	}
	//
	return RET_SUCCESS;
}
float StreamFileJoiner::Flush( void )
{
	if( _joined < _size )
	{
		FileChunk *chunk;
		size_t item_count, items_written;
		//
		while( !_chunks.empty() && (chunk = &_chunks.front())->ID == (_lastIDWritten + 1) )
		{
			item_count = chunk->Size/_itemSize;
			//std::cout<<chunk->ID<<": "<<chunk->Size<<": "<<chunk->Payload<<"\n";
			items_written = ::fwrite( chunk->Payload, _itemSize, item_count, _file );
			//
			//std::cout<<"actual: "<<item_count<<", written: "<<items_written<<"\n";
			if( items_written != item_count )
			{
				if( ::ferror(_file) )
				{
					std::cout<<"[ERR: StreamFileJoiner: 2]: Could not finish fwrite\n";
					return RET_BAD_DESTINATION;
				}
			}
			_lastIDWritten = chunk->ID;
			_joined += chunk->Size;
			_listSize -= chunk->Size;
			delete chunk->Payload;
			_chunks.pop_front();
		}
		if( _joined == _size )
		{
			::fclose( _file );
			_file = 0;
		}
	}
	//
	return ( _joined/_size );
}
bool StreamFileJoiner::IsDone( void )
{
	return _joined == _size;
}
long long StreamFileJoiner::TotalSize( void )
{
	return _size;
}
long long StreamFileJoiner::CurrentTotalSize( void )
{
	return _joined;
}