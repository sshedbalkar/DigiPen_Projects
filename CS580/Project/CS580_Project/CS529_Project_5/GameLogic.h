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
//
namespace Wye
{
	class Message;
	typedef ObjectLinkList<Controller> ControllerListType;
	//
	///Sample Demo Game Logic
	class GameLogic: public System
	{
//#pragma message("Including Class: GameLogic")
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
		Vec3 WorldMousePosition;
		unsigned currentTime;
		//
	private:
		ControllerListType _controllers;
		//
		void drawInfo();
		void createDummyCamera();
		bool _ended;
		bool _paused;
	};
	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* LOGIC;
}
//
#endif
