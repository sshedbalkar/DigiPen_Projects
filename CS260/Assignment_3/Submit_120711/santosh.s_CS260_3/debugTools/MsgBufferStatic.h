#pragma once
//
#include "MsgBuffer.h"
//
namespace debug
{
	#define MAX_MSG_LENGTH			1000
	#define MAX_CONCURRENT_MSGS		1000
	//
	class MsgBufferStatic: public MsgBuffer
	{
	public:
		MsgBufferStatic();
		virtual ~MsgBufferStatic();
		//
		void PushBack( const char *msg );
		const char* PopFront();
		bool IsEmpty();
		//
	private:
		char m_buffer[ MAX_CONCURRENT_MSGS + 1 ][ MAX_MSG_LENGTH ];
		int m_head;
		int m_tail;
	};
}