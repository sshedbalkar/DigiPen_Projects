#ifndef TEST_COMPONENT_H
#define TEST_COMPONENT_H
//#pragma message("Compiling: "__FILE__)
//
#include "Precompiled.h"
//
using namespace Wye;
class TComp: public Composition
{
private:
	Composition C1;
public:
	TComp()
	{
		C1.setParent(this);
		std::cout << "[TComp:" << getId() << "]: Constructed \n";
	};
	~TComp()
	{
		std::cout << "[TComp:" << getId() << "]: Destructed \n";
	};
	//
	void onMessage(const Message& msg)
	{
		std::cout << "[TComp:" << getId() << "]: Got Message: " << msg.MessageId << "\n";
	};
	void onEvent(Event* evt)
	{
		std::cout << "[TComp:" << getId() << "]: onEvent: " << evt->MessageId << "\n";
	};
	void childDispatch()
	{
		Event e1(Msgid::Msg_1, this, true, false);
		std::cout<<"Doing child dispatch\n";
		C1.dispatchEvent(&e1);
	};
	void parentListen()
	{
		Composition* prnt = getParent();
		if(prnt)
		{
			std::cout<<"Added Parentttttttttttt listener\n";
			prnt->addEventListener(Msgid::Msg_1, this, &TComp::onEvent, -1);
		}
	};
	void removeparentListener()
	{
		Composition* prnt = getParent();
		if(prnt)
		{
			std::cout<<"REMOVED Parentttttttttttt listener\n";
			prnt->addEventListener(Msgid::Msg_1, this, &TComp::onEvent);
		}
	}
};
//
#endif