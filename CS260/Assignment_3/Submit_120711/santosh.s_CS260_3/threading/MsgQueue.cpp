#include "MsgQueue.h"
#include <iostream>
#include "../common/ReturnCodes.h"
#include "../debugTools/Assert.h"
//
MsgQueue::MsgQueue()
	:_event( 0 )
{
	//
}
MsgQueue::~MsgQueue()
{
	//::CloseHandle( _event );
	::DeleteCriticalSection( &_crit );
}
int MsgQueue::Init()
{
	::InitializeCriticalSection( &_crit );
	//
	/*
	_event = CreateEvent( NULL, true, false, NULL );
	if( _event == NULL )
	{
		std::cout<<"Error creating Event: "<<GetLastError()<<"\n";
		return GENERIC_FAIL;
	}
	*/
	//
	return RET_SUCCESS;
}
int MsgQueue::Push( Message *const msg )
{
	::EnterCriticalSection( &_crit );
	//std::cout<<"Pushing Msg: "<<msg->GetName()<<"\n";
	_queue.push( msg );
	::SetEvent( _event );
	::LeaveCriticalSection( &_crit );
	//
	return RET_SUCCESS;
}
Message* MsgQueue::Front()
{
	Message *msg = 0;
	::EnterCriticalSection( &_crit );
	if( !_queue.empty() )
	{
		msg = _queue.front();
		_queue.pop();
	}
	::LeaveCriticalSection( &_crit );
	return msg;
}
bool MsgQueue::IsEmpty()
{
	/*
	DWORD result = ::WaitForSingleObject( _event, 0 );
	switch( result )
	{
	case WAIT_OBJECT_0:
		{
			::ResetEvent( _event );
			return false;
		}
		break;
	case WAIT_TIMEOUT:
		{
			return true;
		}
		break;
	default:
		MCR_Assert( false, "Wait for event failed" );
	}
	return true;
	*/
	::EnterCriticalSection( &_crit );
	bool b = _queue.empty();
	::LeaveCriticalSection( &_crit );
	return b;
}