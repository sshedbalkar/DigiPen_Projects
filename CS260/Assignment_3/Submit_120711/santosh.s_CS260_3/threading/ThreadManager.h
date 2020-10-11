#pragma once
//
#include "../common/Singleton.h"
#include <vector>
#include "../os/WindowsIncludes.h"
//
class Thread;
class FunctionObject;
//
class ThreadManager: public Singleton< ThreadManager >
{
public:
									ThreadManager();
									~ThreadManager();
	//
	int								Cleanup();
	Thread*							CreateNewThread( void *obj, FunctionObject *func, CRITICAL_SECTION *&cs, void *data );
	int								Add( Thread *t );
	int								Remove( Thread *t );
	int								DeleteAllThreads();
	int								DeleteThread( Thread *t );
	//
private:
	typedef std::vector< Thread* >	ThreadList;
	ThreadList::iterator			Find( Thread *t );
	int								Remove( ThreadList::iterator &it );
private:
	ThreadList						_threads;
};