///////////////////////////////////////////////////////////////////////////////////////
//
//	Game Logic
//	Simple game logic for demo. A good way to approach understand the game sample
//	is to walk through all the code in game logic and follow the execution through
//	the rest of the engine.
//	
//	Authors: , 
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "GameLogic.h"
#include "Physics.h"
#include "Graphics.h"
#include "Audio.h"
#include "WindowsSystem.h"
#include "Core.h"
#include "Utility.h"
#include <ctime>
#include "particleEmitter.h"
#include "LoadLevelClass.h"
#include "Level_1.h"
#include "Level_2.h"
#include "Level_3.h"
#include "debugGeometry.h"
#include "GSM.h"
#include "ILevel.h"
#include "LevelEditor.h"
#include "ComponentCreator.h"
#include "ActivationProperties.h"
#include "ReturnCodes.h"
#include "cameraclass.h"
//#include "Integration.h"

namespace Framework
{
	//Our global pointer to the game.
	GameLogic* LOGIC = NULL;
	ILevel* l1;						//#### added temporary line


	void GameLogic::Initialize()
	{
		RegisterComponent(ActivationProperties);
		l1 = new Level_1();					//#### added temporary line
		GRAPHICS->SetSceneNum( 1 );
		CORE->gsm()->initialize(l1);
		GRAPHICS->TurnOnDebugCamera();
		CAMERA->position = Vec3(0,20,-100);
		Vector3 a1=-CAMERA->position;
		a1.normalise();
		CAMERA->m_lookAt = Vector3ToVec3(a1);
		srand(time(NULL));
	}

	GameLogic::GameLogic():
		_paused(true),
		m_levelId( 0 )
	{
		ErrorIf(LOGIC!=NULL,"Logic already initialized");
		LOGIC = this;
	}

	GameLogic::~GameLogic()
	{
		delete l1;							//#### added temporary line
	}

	static float volume=0.0f; //temp code remove later
	//static bool playsounds=true; //temp code remove later

	int GameLogic::LoadNextLevel()
	{
		m_levelId = (m_levelId + 1) % 3;
		if( m_levelId == 0 )
		{
			l1 = new Level_1();
		}
		else if( m_levelId == 1 )
		{
			l1 = new Level_2();
		}
		else if( m_levelId == 2 )
		{
			l1 = new Level_3();
		}
		//
		CORE->gsm()->next( l1 );
		//
		return RET_SUCCESS;
	}
	void GameLogic::SendMessage(Message * m )
	{
		if(EDITOR && EDITOR->inEditor)return;
		switch( m->MessageId )
		{
		case Mid::CharacterKey:
			{
				l1->sendMessage( m );
				MessageCharacterKey * key = (MessageCharacterKey*)m;
				if(key->character==VK_ESCAPE)
				{
					Message m(Mid::Quit);
					CORE->BroadcastMessage(&m);
				}else if(key->character=='`' && EDITOR)
				{
					EDITOR->enterEditMode();
					GRAPHICS->TurnOnDebugCamera();
				}
				else if(key->character=='n')
				{
					LoadNextLevel();
				}
				break;
			}
		case Mid::MouseMove:
			{
				MouseMove * mouseMove = (MouseMove*)m;
				mousepos=Vector3(mouseMove->MousePosition.x,mouseMove->MousePosition.y,0.0);
				mousedir=Vector3(0,0,0);
				GRAPHICS->ScreenToWorldSpaceRay(mousepos,mousedir);
				break;
			}
		case Mid::MouseWheel :
			{
				break;
			}
		case Mid::SystemPause:
			{
				const SystemPause *msg = static_cast<const SystemPause*>(m);
				_paused = msg->Pause;
				break;
			}
		}
	}

	void GameLogic::Update(float dt)
	{
		GRAPHICS->SetSceneNum( m_levelId + 1 );
		if(EDITOR && EDITOR->inEditor)return;
		return;
	}
	
}
