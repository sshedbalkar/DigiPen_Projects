#pragma once

#include "statemch.h"


class SM_Runner_1 : public StateMachine
{
public:

	SM_Runner_1( GameObject & object, int team, int role )
		: StateMachine( object ), m_roleId( role ) {}
	~SM_Runner_1( void ) {}


private:

	virtual bool States( State_Machine_Event event, MSG_Object * msg, int state, int substate );

	//Put member variables or functions here
	objectID m_curTarget;
	//objectID GetFarthestAgent( void );
	int m_roleId;
};