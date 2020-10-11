#include "DXUT.h"
#include "SM_Runner_3.h"
#include "database.h"
#include "movement.h"
#include "body.h"
#include <iostream>
#include "SM_Announcer.h"

//Add new states below
enum StateName {
	STATE_Initialize,	//The first enum is the starting state
	STATE_Idle,
	STATE_MoveToPosStart,
	STATE_Run,
	STATE_Run_0,
	STATE_Run_1,
	STATE_Run_2,
	STATE_Run_3,
	STATE_Switch_1,
	STATE_Switch_2,
	STATE_Nothing
};

//Add new substates below
enum SubstateName {		//Order doesn't matter
	SUB_Go_Up,
	SUB_Go_Down
};

bool SM_Runner_3::States( State_Machine_Event event, MSG_Object * msg, int state, int substate )
{
BeginStateMachine
	OnMsg( MSG_MoveToStart )
		ChangeStateDelayed( RandDelay( 1.0f, 3.0f ), STATE_MoveToPosStart );
	//////////////////////////////////////////////
	DeclareState( STATE_Initialize )
		OnEnter
			m_owner->GetMovement().SetIdleSpeed();
			ChangeStateDelayed( 1.0f, STATE_Idle );
			std::cout<<"ID: "<<m_owner->GetID()<<", role: "<<m_roleId<<"\n";
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Idle )
		OnEnter
			m_owner->GetMovement().SetIdleSpeed();
		OnMsg( MSG_Switch_1 )
			ChangeStateDelayed( 1.0f, STATE_Switch_1 );
		OnMsg( MSG_Switch_2 )
			std::cout<<"3 switch 2\n";
			ChangeStateDelayed( 1.0f, STATE_Switch_2 );
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Switch_2 )
		OnEnter
			ChangeSubstate( SUB_Go_Up );
			SetCCReceiver( m_owner->GetID() );
		OnCCMsg( MSG_Switch_1_1_Done )
			ChangeSubstate( SUB_Go_Down );
		OnCCMsg( MSG_Switch_1_2_Done )
			ChangeState( STATE_Idle );
			SendMsg(MSG_Switch_2_Done, ANNOUNCER->GetID());
		OnExit
			ClearCCReceiver();
		DeclareSubstate( SUB_Go_Up )
			OnTimeInSubstate(0.5f)
				m_owner->GetMovement().SetWalkSpeed();
				D3DXVECTOR3 target( 0, 0, 0 );
				if( m_roleId == 0 )
				{
					target.x = 1.0f;
					target.z = 0.5f;
				}
				else if( m_roleId == 1 )
				{
					target.x = 0.5f;
					target.z = 1.0f;
				}
				else if( m_roleId == 2 )
				{
					target.x = 0.0f;
					target.z = 0.5f;
				}
				else if( m_roleId == 3 )
				{
					target.x = 0.5f;
					target.z = 0.0f;
				}
				m_owner->GetMovement().SetTarget( target );
			OnMsg( MSG_Arrived )
				m_owner->GetMovement().SetIdleSpeed();
				SendMsgToState( MSG_Switch_1_1_Done );
		DeclareSubstate( SUB_Go_Down )
			OnTimeInSubstate(0.5f)
				m_owner->GetMovement().SetWalkSpeed();
				D3DXVECTOR3 target( 0, 0, 0 );
				if( m_roleId == 0 )
				{
					target.x = 0.5f;
					target.z = 1.0f;
				}
				else if( m_roleId == 1 )
				{
					target.x = 0.0f;
					target.z = 0.5f;
				}
				else if( m_roleId == 2 )
				{
					target.x = 0.5f;
					target.z = 0.0f;
				}
				else if( m_roleId == 3 )
				{
					target.x = 1.0f;
					target.z = 0.5f;
				}
				m_owner->GetMovement().SetTarget( target );
			OnMsg( MSG_Arrived )
				m_owner->GetMovement().SetIdleSpeed();
				SendMsgToState( MSG_Switch_1_2_Done );
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Switch_1 )
		OnEnter
			ChangeSubstate( SUB_Go_Up );
			SetCCReceiver( m_owner->GetID() );
		OnCCMsg( MSG_Switch_1_1_Done )
			ChangeSubstate( SUB_Go_Down );
		OnCCMsg( MSG_Switch_1_2_Done )
			ChangeState( STATE_Idle );
			SendMsg(MSG_Switch_1_Done, ANNOUNCER->GetID());
		OnExit
			ClearCCReceiver();
		DeclareSubstate( SUB_Go_Up )
			OnTimeInSubstate(0.5f)
				m_owner->GetMovement().SetWalkSpeed();
				D3DXVECTOR3 target( 0, 0, 0 );
				if( m_roleId == 3 )
				{
					target.x = 0.8f;
					target.z = 0.5f;
				}
				else if( m_roleId == 0 )
				{
					target.x = 0.5f;
					target.z = 0.8f;
				}
				else if( m_roleId == 1 )
				{
					target.x = 0.2f;
					target.z = 0.5f;
				}
				else if( m_roleId == 2 )
				{
					target.x = 0.5f;
					target.z = 0.2f;
				}
				m_owner->GetMovement().SetTarget( target );
			OnMsg( MSG_Arrived )
				m_owner->GetMovement().SetIdleSpeed();
				SendMsgToState( MSG_Switch_1_1_Done );
		DeclareSubstate( SUB_Go_Down )
			OnTimeInSubstate(0.5f)
				m_owner->GetMovement().SetWalkSpeed();
				D3DXVECTOR3 target( 0, 0, 0 );
				if( m_roleId == 3 )
				{
					target.x = 1.0f;
					target.z = 0.5f;
				}
				else if( m_roleId == 0 )
				{
					target.x = 0.5f;
					target.z = 1.0f;
				}
				else if( m_roleId == 1 )
				{
					target.x = 0.0f;
					target.z = 0.5f;
				}
				else if( m_roleId == 2 )
				{
					target.x = 0.5f;
					target.z = 0.0f;
				}
				m_owner->GetMovement().SetTarget( target );
			OnMsg( MSG_Arrived )
				m_owner->GetMovement().SetIdleSpeed();
				SendMsgToState( MSG_Switch_1_2_Done );
	
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_MoveToPosStart )
		OnEnter
			//m_owner->GetMovement().SetJogSpeed();
			m_owner->GetMovement().SetWalkSpeed();
			D3DXVECTOR3 target( 0, 0, 0 );
			if( m_roleId == 0 )
			{
				target.x = 1.0f;
				target.z = 0.5f;
			}
			else if( m_roleId == 1 )
			{
				target.x = 0.5f;
				target.z = 1.0f;
			}
			else if( m_roleId == 2 )
			{
				target.x = 0.0f;
				target.z = 0.5f;
			}
			else if( m_roleId == 3 )
			{
				target.x = 0.5f;
				target.z = 0.0f;
			}
			m_owner->GetMovement().SetTarget( target );
		OnMsg( MSG_Arrived )
			m_owner->GetMovement().SetIdleSpeed();
			//std::cout<<"Sending ready to run: "<<m_owner->GetID()<<"\n";
			SendMsg(MSG_ReadyToRun, ANNOUNCER->GetID());
		OnMsg( MSG_Run )
			//std::cout<<"hello3\n";
			ChangeState( STATE_Run );

	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Run )
		OnEnter
			m_owner->GetMovement().SetIdleSpeed();
			if( m_roleId == 0 )
				ChangeStateDelayed(0.5f, STATE_Run_0);
			//std::cout<<"Rcvd msg to run\n";
		OnMsg( MSG_Continue_Run )
			int next = msg->GetIntData();
			if( next == 1 && m_roleId == 1 )
				ChangeState(STATE_Run_1);
			else if( next == 2 && m_roleId == 2 )
				ChangeState(STATE_Run_2);
			else if( next == 3 && m_roleId == 3 )
				ChangeState(STATE_Run_3);
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	DeclareState( STATE_Run_0 )
		DeclareStateFloat( midX )
		DeclareStateFloat( midZ )
		DeclareStateFloat( finalX )
		DeclareStateFloat( finalZ )
		DeclareStateFloat( vel )
		DeclareStateInt( mode )
		//
		OnEnter
			mode = 0;
			midX = 1.0f;
			midZ = 1.0f;
			//
			finalX = 0.5f;
			finalZ = 1.0f;
			//
			vel = RandDelay( 0.75f, 1.25f );
			m_owner->GetMovement().SetJogSpeed( vel );
			D3DXVECTOR3 target( midX, 0, midZ );
			m_owner->GetMovement().SetTarget( target );
			//
			OnMsg( MSG_Arrived )
				++mode;
				if( mode == 1 )
				{
					D3DXVECTOR3 target( finalX, 0, finalZ );
					m_owner->GetMovement().SetTarget( target );
				}
				else if( mode == 2 )
				{
					m_owner->GetMovement().SetIdleSpeed();
					GameObject *go = g_database.Find( m_owner->GetID() + 1 );
					if( go != NULL )
					{
						//std::cout<< "requesting to continue: "<<m_owner->GetID()<<"\n";
						MSG_Data d(1);
						SendMsg( MSG_Continue_Run, go->GetID(), d );
					}
					ChangeState( STATE_Idle );
				}
	//////////////////////////////////////////////
	
	//////////////////////////////////////////////
	DeclareState( STATE_Run_1 )
		DeclareStateFloat( midX )
		DeclareStateFloat( midZ )
		DeclareStateFloat( finalX )
		DeclareStateFloat( finalZ )
		DeclareStateFloat( vel )
		DeclareStateInt( mode )
		//
		OnEnter
			mode = 0;
			midX = 0.0f;
			midZ = 1.0f;
			//
			finalX = 0.0f;
			finalZ = 0.5f;
			//
			vel = RandDelay( 0.75f, 1.25f );
			m_owner->GetMovement().SetJogSpeed( vel );
			D3DXVECTOR3 target( midX, 0, midZ );
			m_owner->GetMovement().SetTarget( target );
			//
			OnMsg( MSG_Arrived )
				++mode;
				if( mode == 1 )
				{
					D3DXVECTOR3 target( finalX, 0, finalZ );
					m_owner->GetMovement().SetTarget( target );
				}
				else if( mode == 2 )
				{
					m_owner->GetMovement().SetIdleSpeed();
					GameObject *go = g_database.Find( m_owner->GetID() + 1 );
					if( go != NULL )
					{
						//std::cout<< "requesting to continue: "<<m_owner->GetID()<<"\n";
						MSG_Data d(2);
						SendMsg( MSG_Continue_Run, go->GetID(), d );
					}
					ChangeState( STATE_Idle );
				}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Run_2 )
		DeclareStateFloat( midX )
		DeclareStateFloat( midZ )
		DeclareStateFloat( finalX )
		DeclareStateFloat( finalZ )
		DeclareStateFloat( vel )
		DeclareStateInt( mode )
		//
		OnEnter
			mode = 0;
			midX = 0.0f;
			midZ = 0.0f;
			//
			finalX = 0.5f;
			finalZ = 0.0f;
			//
			vel = RandDelay( 0.75f, 1.25f );
			m_owner->GetMovement().SetJogSpeed( vel );
			D3DXVECTOR3 target( midX, 0, midZ );
			m_owner->GetMovement().SetTarget( target );
			//
			OnMsg( MSG_Arrived )
				++mode;
				if( mode == 1 )
				{
					D3DXVECTOR3 target( finalX, 0, finalZ );
					m_owner->GetMovement().SetTarget( target );
				}
				else if( mode == 2 )
				{
					m_owner->GetMovement().SetIdleSpeed();
					GameObject *go = g_database.Find( m_owner->GetID() + 1 );
					if( go != NULL )
					{
						//std::cout<< "requesting to continue: "<<m_owner->GetID()<<"\n";
						MSG_Data d(3);
						SendMsg( MSG_Continue_Run, go->GetID(), d );
					}
					ChangeState( STATE_Idle );
				}
	//////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Run_3 )
		DeclareStateFloat( midX )
		DeclareStateFloat( midZ )
		DeclareStateFloat( finalX )
		DeclareStateFloat( finalZ )
		DeclareStateFloat( vel )
		DeclareStateInt( mode )
		//
		OnEnter
			mode = 0;
			midX = 1.0f;
			midZ = 0.0f;
			//
			finalX = 1.0f;
			finalZ = 0.5f;
			//
			vel = RandDelay( 0.75f, 1.25f );
			m_owner->GetMovement().SetJogSpeed( vel );
			D3DXVECTOR3 target( midX, 0, midZ );
			m_owner->GetMovement().SetTarget( target );
			//
			OnMsg( MSG_Arrived )
				++mode;
				if( mode == 1 )
				{
					D3DXVECTOR3 target( finalX, 0, finalZ );
					m_owner->GetMovement().SetTarget( target );
				}
				else if( mode == 2 )
				{
					m_owner->GetMovement().SetIdleSpeed();
					GameObject *go = g_database.Find( m_owner->GetID() + 1 );
					if( go != NULL )
					{
						//std::cout<< "requesting to continue: "<<m_owner->GetID()<<"\n";
						//
						SendMsg(MSG_Finished_Run, ANNOUNCER->GetID());
					}
					ChangeState( STATE_Idle );
				}
	/////////////////////////////////////////////

	//////////////////////////////////////////////
	DeclareState( STATE_Nothing )
		OnEnter
			m_owner->GetMovement().SetIdleSpeed();
			
	//////////////////////////////////////////////

EndStateMachine
}

