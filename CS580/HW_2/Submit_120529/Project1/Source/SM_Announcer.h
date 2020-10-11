#pragma once

#include "statemch.h"


class SM_Announcer : public StateMachine
{
public:

	SM_Announcer( GameObject & object )
		: StateMachine( object ) {}
	~SM_Announcer( void ) {}


private:

	virtual bool States( State_Machine_Event event, MSG_Object * msg, int state, int substate );

	//Put member variables or functions here
	objectID m_curTarget;
	void createBroadcastList();

};
extern GameObject* ANNOUNCER;