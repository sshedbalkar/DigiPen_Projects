///////////////////////////////////////////////////////////////////////////////
///
///	\file Message.h
///	Object Messaging System used by engine for cross game object and system 
///	communication. Messages are passed as base class pointers and downcast 
///	to their derieved types by the receiver usually inside of a switch
///	block.
///
///	Authors: Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#ifndef MESSAGE_H
#define MESSAGE_H
//#pragma message("Compiling: "__FILE__)
//
#include "Object.h"
#include "MsgIds.h"

namespace Wye
{
	///Base message class. New message types are defined by deriving from this
	///class and mapping defined an element of the MessageIdType enumeration.
	class Message: public Object
	{
#pragma message("Including Class: Message")
	public:
		Message(Msgid::MessageIdType id);
		Message();
		Message(const Message& orig);
		virtual ~Message();
		//
		Message& operator =(const Message& rhs);
		//
		Msgid::MessageIdType MessageId;
		//
	};
	//
	class Event: public Message
	{
#pragma message("Including Class: Event")
	public:
		Event(Msgid::MessageIdType id, Object* target, bool bubbles = false, bool cancelable = false);
		Event(const Event& orig);
		~Event();
		//
		Event& operator =(const Event& rhs);
		//
		virtual void stopPropagation();
		virtual bool bubbles() const;
		virtual bool cancelable() const;
		virtual Object* getCurrentTarget();
		virtual void setCurrentTarget(Object* currentTarget);
		virtual Object* target();
		virtual bool canceled() const;
	protected:
		Object* _currentTarget;
		Object* _target;
		bool _bubbles;
		bool _cancelable;
		bool _canceled;
	};
}
//
#endif