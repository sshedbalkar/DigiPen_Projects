///////////////////////////////////////////////////////////////////////////////////////
///
///	\file GameLogic.h
///	Sample game logic for demo.
///	
///	Authors: Benjamin Ellinger, Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////

#pragma once //Makes sure this header is only included once

#include "Graphics.h"
#include "Physics.h"
#include "Engine.h"
#include "GameData.h"
#include "VMath.h"
//
namespace Framework
{
	
	class Rays
	{
		LineListType* line_list;
		int numberOfRays;
	public:
		Rays();
		~Rays(){delete line_list;}
		void Update(Vec2 heroPos,Vec2 mousePos);
		void DestroyRays();
		void CollisionCheck();
		float FindX(float y);
		float FindY(float x);
		float SquaredDistance(Vec2 a,Vec2 b);
	};

	///Sample Demo Game Logic
	class GameLogic : public ISystem
	{
	public:	
		GameLogic();
		~GameLogic();
		GOC* HERO;
		GOC* CAMERA;
		void Initialize();
		void Update(float timeslice);
		virtual std::string GetName(){return "GameLogic";}
		virtual void SendMessage(Message *);
		//GOC * CreateObjectAt(Vec2& position,float rotation,const std::string& file);
		GOC* CreateObjectAt(const char* archId, Vec2& position, float rotation, Vec3& scale);
		GOC* CreateObjectAt(const char* archId, Vec2& position, float rotation);
		GOC* CreateObjectAt(const char* archId, Vec2& position);
		void LoadLevelFile(const std::string& file);
		unsigned GrabbedObjectId;
		Vec2 WorldMousePosition;
	private:
		Rays *ray;
		GameData* _gameData;
		void loadGameSettings(const std::string& file);

	};

	//A global pointer to our game so that it can be accessed from anywhere.
	extern GameLogic* LOGIC;
}


