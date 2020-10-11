#include "MsgBufferStatic.h"
#include <string>
//
namespace debug
{
	MsgBufferStatic::MsgBufferStatic()
		:m_head( 0 ),
		m_tail( 0 )
	{
		memset( m_buffer, 0, MAX_CONCURRENT_MSGS*MAX_MSG_LENGTH );
	}
	MsgBufferStatic::~MsgBufferStatic()
	{
		m_head = m_tail = -2;
	}
	void MsgBufferStatic::PushBack( const char *msg )
	{
		strcpy_s( m_buffer[ m_tail ], MAX_MSG_LENGTH, msg );
		m_tail = ( m_tail == MAX_CONCURRENT_MSGS ) ? 0 : m_tail + 1;
	}
	const char* MsgBufferStatic::PopFront()
	{
		int i = m_head;
		m_head = ( m_head == MAX_CONCURRENT_MSGS ) ? 0 : m_head + 1;
		return m_buffer[ m_head ];
	}
	bool MsgBufferStatic::IsEmpty()
	{
		return m_head == m_tail;
	}
}