#pragma once
//
#include "../os/WindowsIncludes.h"
#include <queue>
#include "../message/Message.h"
//
//class Message;
//
class MsgQueue
{
public:
									MsgQueue();
	virtual							~MsgQueue();
	//
	int								Init();
	int								Push( Message *const msg );
	Message*						Front();
	bool							IsEmpty();
	//
private:
	std::queue< Message* >			_queue;
	CRITICAL_SECTION				_crit;
	HANDLE							_event;
};