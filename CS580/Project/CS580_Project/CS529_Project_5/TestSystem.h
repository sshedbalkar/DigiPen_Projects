#ifndef TEST_SYSTEM_H
#define TEST_SYSTEM_H
//#pragma message("Compiling: "__FILE__)
//
#include <iostream>
#include "Precompiled.h"
#include "TestComponent.h"
//
using namespace Wye;
class TSys: public System
{
public:
	TComp t1, t2;
	unsigned f;
	TSys():
	t1(),
	t2(),
	f(0)
	{
		t1.setParent(this);
		t2.setParent(this);
		std::cout << "[TSys:" << getId() << "]: Constructed \n";
	};
	~TSys()
	{
		std::cout << "[TSys:" << getId() << "]: Destructed \n";
	};
	//
	void initialize()
	{
		std::cout << "[TSys:" << getId() << "]: Init \n";
	};
	void onMessage(const Message& msg)
	{
		std::cout << "[TSys:" << getId() << "]: Got Message: " << msg.MessageId << "\n";
	};
	void onEvent(Event* evt)
	{
		std::cout << "[TSys:" << getId() << "]: onEvent: " << evt->MessageId << "\n";
	};
	void update(float timeslice)
	{
		//std::cout << "[TSys:" << getId() << "]: Updating \n";
		++f;
		if(f == 20)
		{
			//t1.addEventListener(Msgid::MessageIdType::Msg_1, this, &TSys::onEvent, -1);
			t2.parentListen();
			std::cout<< "Added event listener\n";
		}
		if(f == 200)
		{
			std::cout<< "Dispatching event\n";
			//Event e1(Msgid::Msg_1, &t1, true, false);
			//t1.dispatchEvent(&e1);
			//std::cout<< "Removing listener\n";
			//t1.removeEventListener(Msgid::MessageIdType::Msg_1, this, &TSys::onEvent);
			t1.childDispatch();
		}
		if(f == 300)
		{
			t2.removeparentListener();
		}
	};
};
//
#endif