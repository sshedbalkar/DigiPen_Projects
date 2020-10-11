//#pragma message("Compiling: "__FILE__)
//
#include "Message.h"
#include "Utility.h"
//
namespace Wye
{
	Message::Message(Msgid::MessageIdType id): 
		Object(getUniqueId()), 
		MessageId(id)
	{
		//
	}
	
	Message::Message(): 
		Object(getUniqueId()), 
		MessageId(Msgid::None)
	{
		//
	}
	Message::Message(const Message& orig):
		Object(orig.getId()),
		MessageId(orig.MessageId)
	{
		//
	}
	Message::~Message()
	{
		//
	}
	//
	Message& Message::operator =(const Message& rhs)
	{
		MessageId = rhs.MessageId;
		Object::operator=(rhs);
		return *this;
	}
	//
	Event::Event(Msgid::MessageIdType id, Object* target, bool bubbles, bool cancelable): 
		Message(id), 
		_currentTarget(target), 
		_target(target), 
		_bubbles(bubbles), 
		_cancelable(cancelable), 
		_canceled(false)
	{
		//
	}
	Event::Event(const Event& orig): 
		Message(orig), 
		_currentTarget(orig._currentTarget), 
		_target(orig._target), 
		_bubbles(orig._bubbles), 
		_cancelable(orig._cancelable), 
		_canceled(orig._canceled)
	{
		//
	}
	Event::~Event()
	{
		//
	}
	//
	Event& Event::operator=(const Event& rhs)
	{
		_currentTarget = rhs._currentTarget;
		_target = rhs._target, 
		_bubbles = rhs._bubbles;
		_cancelable = rhs._cancelable;
		_canceled = rhs._canceled;
		Message::operator=(rhs);
		return *this;
	}
	//
	Object* Event::getCurrentTarget()
	{
		return _currentTarget;
	}
	void Event::setCurrentTarget(Object* currentTarget)
	{
		_currentTarget = currentTarget;
	}
	Object* Event::target()
	{
		return _target;
	}
	void Event::stopPropagation()
	{
		_canceled = cancelable();
	}
	bool Event::bubbles() const
	{
		return _bubbles;
	}
	bool Event::cancelable() const
	{
		return _cancelable;
	}
	bool Event::canceled() const
	{
		return _canceled;
	}
}