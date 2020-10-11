#include "Thread.h"
#include "../common/ReturnCodes.h"
#include <iostream>
#include "../common/FunctionBind.h"
#include "MsgQueue.h"
#include "../debugTools/Assert.h"
//
ThreadInfo::ThreadInfo( CRITICAL_SECTION *& cs )
	:data( 0 ),
	callback( 0 ),
	object( 0 ),
	_crit( cs )
{
	//
}
ThreadInfo::~ThreadInfo()
{
	delete callback;
	delete _msgs;
}
int ThreadInfo::Init()
{
	_msgs = new MsgQueue();
	return _msgs->Init();
}
MsgQueue* ThreadInfo::Msgs()
{
	return _msgs;
}
CRITICAL_SECTION* ThreadInfo::SyncObj()
{
	return _crit;
}
///////////////////////////////////////////////////////////////////////////////////////////
Thread::Thread( ThreadInfo *&info )
	:_info( info ),
	_active( true ),
	_handle( 0 )
{
	//
}
Thread::~Thread()
{
	MCR_Assert( _handle != NULL, "Thread handle is null" );
	::CloseHandle( _handle );
	delete _info;
	//std::cout<<"Closed thread: "<<_handle<<"\n";
}
int Thread::Init()
{
	_handle = ::CreateThread( NULL, 0, &Thread::StaticThreadStart, ( void* )this, 0, &_id );
	if( _handle == NULL )
	{
		std::cout<<"Error creating thread: "<<::GetLastError()<<"\n";
		return RET_FAIL;
	}
	return RET_SUCCESS;
}
DWORD Thread::ThreadStart()
{
	ArgumentVector v1;
	v1.push_back( _info->data );
	v1.push_back( this );
	Variant r = _info->callback->Invoke( _info->object, v1 );
	_active = false;
	return r.As< DWORD >();
}
DWORD WINAPI Thread::StaticThreadStart( LPVOID param )
{
	Thread* This = ( Thread* ) param;
	return This->ThreadStart();
}
HANDLE Thread::Handle()
{
	return _handle;
}
int Thread::StartSync()
{
	::EnterCriticalSection( _info->SyncObj() );
	return RET_SUCCESS;
}
int Thread::EndSync()
{
	::LeaveCriticalSection( _info->SyncObj() );
	return RET_SUCCESS;
}
bool Thread::IsActive()
{
	return _active;
}
MsgQueue* Thread::Messages()
{
	return _info->Msgs();
}