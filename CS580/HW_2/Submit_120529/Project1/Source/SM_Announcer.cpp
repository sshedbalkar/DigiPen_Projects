#include "DXUT.h"
#include "SM_Announcer.h"
#include "database.h"
#include "movement.h"
#include "body.h"



//Add new states below
enum StateName {
	STATE_Initialize,	//The first enum is the starting state
	STATE_Idle,
	STATE_Announce,
	STATE_Wait_For_Finish
};

//Add new substates below
enum SubstateName {		//Order doesn't matter
	//empty
};

GameObject* ANNOUNCER;

bool SM_Announcer::States( State_Machine_Event event, MSG_Object * msg, int state, int substate )
{
BeginStateMachine
	//////////////////////////////////////////////
	DeclareState( STATE_Initialize )
		OnEnter
			ANNOUNCER = m_owner;
			m_owner->GetMovement().SetIdleSpeed();
			ChangeStateDelayed( 1.0f, STATE_Announce );
		OnExit
			createBroadcastList();
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Idle )
		OnEnter
			m_owner->GetMovement().SetIdleSpeed();
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Announce )
		DeclareStateInt( unitsReady )
		OnEnter
			SendMsgBroadcastToList( MSG_MoveToStart );
			unitsReady = 0;
		OnMsg( MSG_ReadyToRun )
			++unitsReady;
			//std::cout<<"Rcvd ready to run: "<<unitsReady<<"\n";
			if(unitsReady == 12)
			{
				SendMsgDelayedToState(1.0f, MSG_SignalToRun);
			}
		OnMsg(MSG_SignalToRun)
			SendMsgBroadcastToList( MSG_Run );
			ChangeStateDelayed( 1.0f, STATE_Wait_For_Finish );
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Wait_For_Finish )
		DeclareStateInt( unitsFinished )
		OnEnter
			unitsFinished = 0;
		OnMsg( MSG_Finished_Run )
			++unitsFinished;
			if( unitsFinished == 3 )
			{
				unitsFinished = 0;
				SendMsgBroadcastToList( MSG_Switch_1 );
			}
		OnMsg( MSG_Switch_1_Done )
			++unitsFinished;
			if( unitsFinished == 8 )
			{
				unitsFinished = 0;
				SendMsgBroadcastToList( MSG_Switch_2 );
				PopState();
				ChangeStateDelayed( 1.0f, STATE_Idle );
			}
	//////////////////////////////////////////////
EndStateMachine
}
void SM_Announcer::createBroadcastList()
{
	dbCompositionList list;
	g_database.ComposeList( list, OBJECT_NPC );

	BroadcastClearList();

	dbCompositionList::iterator i;
	for( i=list.begin(); i!=list.end(); ++i )
	{
		if( (*i)->GetID() != m_owner->GetID() )
		{
			BroadcastAddToList( (*i)->GetID() );
		}
	}
	//std::cout<<"Created broadcast list\n";
}
