#pragma once
//
#include "../os/WindowsIncludes.h"
#include "ThreadManager.h"
//
class FunctionObject;
class MsgQueue;
//
class ThreadInfo
{
public:
						ThreadInfo( CRITICAL_SECTION *& cs );
	virtual				~ThreadInfo();
	//
	int					Init();
	MsgQueue*			Msgs();
	CRITICAL_SECTION*	SyncObj();
	int					SyncObj( CRITICAL_SECTION *&cs );
	//
public:
	void				*data;
	FunctionObject		*callback;
	void				*object;
	//
private:
	CRITICAL_SECTION	*_crit;
	MsgQueue			*_msgs;
};
//
class Thread
{
public:
	int				StartSync();
	int				EndSync();
	bool			IsActive();
	HANDLE			Handle();
	MsgQueue*		Messages();
	//
	friend class ThreadManager;
	//
private:
					Thread( ThreadInfo *&info );
	virtual			~Thread();
	//
	static DWORD	WINAPI StaticThreadStart( LPVOID param );
	DWORD			ThreadStart();
	int				Init();
	//
private:
	HANDLE			_handle;
	ThreadInfo		*_info;
	DWORD			_id;
	bool			_active;
};