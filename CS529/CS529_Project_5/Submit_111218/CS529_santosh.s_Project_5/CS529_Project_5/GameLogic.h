////////////////////////////////////////////////////////////////
//
//	GameLogic.h
//	Implements high level game logic part
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
//#pragma message("Compiling: "__FILE__)
//
#include "Graphics.h"
#include "System.h"
#include "Containers.h"
#include "Controller.h"
#include "Hero.h"
#include "CollisionMap.h"
//
namespace Wye
{
	class Message;
	typedef ObjectLinkList<Controller> ControllerListType;
	//
	///Sample Demo Game Logic
	class GameLogic: public System
	{
#pragma message("Including Class: GameLogic")
	public:
		GameLogic();
		~GameLogic();
		void initialize();
		void update(float timeslice);
		virtual std::string getName(){return "GameLogic";}
		virtual void onMessage(const Message& msg);
		Composition* createObjectAt(const char* archId, Vec3& position);
		Composition* createObjectAt(const char* archId, Vec3& position, Vec3& rotation);
		Composition* createObjectAt(const char* archId, Vec3& position, Vec3& rotation, Vec3& scale);
		void loadSettingsFile(const char* file);
		void loadLevel(int level);
		void unloadLevel();
		//ControllerListType* getControllers();
		void addComponent(Controller* controller);
		void removeComponent(Controller* controller);
		//unsigned GrabbedObjectId;
		Vec3 WorldMousePosition;
		unsigned currentTime;
		void hero(Hero* hr);
		Hero* hero();
		//
		struct PlayerData
		{
			int Lives;
			int Bullets;
			int Bombs;
			int Level;
			int Coins;
		} playerData;
	private:
		ControllerListType _controllers;
		Hero* _hero;
		CollisionMap _map;
		//
		void drawInfo();
		int _nextLoadLevel;
		void loadLevelI(int level, bool first);
		void createDummyCamera();
		bool _ended;
		bool _paused;
	};
	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* LOGIC;
}
//
#endif
