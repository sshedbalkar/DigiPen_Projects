///////////////////////////////////////////////////////////////////////////////////////
///
///	\file GameLogic.h
///	Sample game logic for demo.
///	
///	Authors: , 
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////

#pragma once //Makes sure this header is only included once
#include "Engine.h"
#include "GameData.h"
#include "VMath.h"
#include "PowerGun.h"
#include <iostream>
#include <vector>
#include "Hero.h"
#include "ObjectActivator.h"
#include "ForceGenerator.h"
//
namespace Framework
{
	class GameLogic : public ISystem
	{
		friend class ObjectActivator;
		friend class LoadLevelClass;
		friend class RigidBody;
	public:	
		GameLogic();
		~GameLogic();
		void Initialize();
		void Update(float timeslice);
		virtual std::string GetName(){return "GameLogic";}
		virtual void SendMessage(Message *);
		GOC* objectPointedAt;
		real objectPointedAtDistance;
		int LoadNextLevel();
	private:
		Hero m_Hero;
		PowerGun m_PowerGun;
		ObjectActivator  m_ObjectActivator;
		enum m_powerState{power1,power2,power3,noOfPower};
		void loadGameSettings(const std::string& file);
		bool _paused;
		std::vector<GOC*> listOfGocs[5];
		std::vector<GOC*> m_ActivatableObjectsList;
		int	m_levelId;
		ForceRegistry reg;
		Vector3 mousepos,mousedir;
	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* LOGIC;
}


