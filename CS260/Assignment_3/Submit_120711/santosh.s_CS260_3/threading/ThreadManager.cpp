#include "ThreadManager.h"
#include "Thread.h"
#include "../common/ReturnCodes.h"
#include "../common/FunctionBind.h"
#include "../debugTools/Assert.h"
#include "../message/Message.h"
#include "../threading/MsgQueue.h"
//
ThreadManager::ThreadManager()
{
	//
}
ThreadManager::~ThreadManager()
{
	DeleteAllThreads();
}
Thread* ThreadManager::CreateNewThread( void *obj, FunctionObject *func, CRITICAL_SECTION *&cs, void *data )
{
	ThreadInfo *info = new ThreadInfo( cs );
	info->object = obj;
	info->callback = func;
	info->data = data;
	if( info->Init() != RET_SUCCESS )
	{
		MCR_Assert( false, "Thread Info init failed" );
	}
	//
	Thread *t = new Thread( info );
	Add( t );
	t->Init();
	//std::cout<<"Created new thread: "<<t->Handle()<<"\n";
	//
	return t;
}
int ThreadManager::DeleteAllThreads()
{
	//std::cout<<"Deleting\n";
	if( Cleanup() > 0 )
	{
		Message *m = new Message( MSG_QUIT, NULL );
		ThreadList::iterator it1 = _threads.begin(), it2 = _threads.end();
		//
		while( it1 != it2 )
		{
			( *it1 )->Messages()->Push( m );
			++it1;
		}
		//
		do
		{
			::Sleep( 10 );
		}
		while( Cleanup() > 0);
	}
	//
	return RET_SUCCESS;
}
int ThreadManager::DeleteThread( Thread *t )
{
	Message *m = new Message( MSG_QUIT, NULL );
	t->Messages()->Push( m );
	while( t->IsActive() ){}
	Cleanup();
	//
	return RET_SUCCESS;
}
int ThreadManager::Add( Thread *t )
{
	if( Find(t) == _threads.end() )
	{
		_threads.push_back( t );
		return RET_SUCCESS;
	}
	return RET_ALREADY_EXISTS;
}
int ThreadManager::Remove( Thread *t )
{
	ThreadList::iterator it = Find( t );
	return Remove( it );
}
ThreadManager::ThreadList::iterator ThreadManager::Find( Thread *t )
{
	ThreadList::iterator it1 = _threads.begin(), it2 = _threads.end(), it;
	bool found = false;
	//
	while( it1 != it2 )
	{
		if( *it1 == t )
		{
			it = it1;
			found = true;
			break;
		}
		++it1;
	}
	//
	return found ? it : it2;
}
int ThreadManager::Remove( ThreadManager::ThreadList::iterator &it )
{
	if( it != _threads.end() )
	{
		_threads.erase( it );
		return RET_SUCCESS;
	}
	return RET_DOESNT_EXIST;
}
int ThreadManager::Cleanup()
{
	if( _threads.size() < 1 ) return 0;
	//
	ThreadList alive;
	ThreadList::iterator it1 = _threads.begin(), it2 = _threads.end();
	//
	while( it1 != it2 )
	{
		if( (*it1)->IsActive() )
		{
			alive.push_back( *it1 );
		}
		else
		{
			//std::cout<<"Deleting thread: "<<(*it1)->Handle()<<"\n";
			delete ( *it1 );
		}
		++it1;
	}
	_threads.swap( alive );
	//std::cout<<"Cleanup, size: "<<_threads.size()<<"\n";
	return _threads.size();
}