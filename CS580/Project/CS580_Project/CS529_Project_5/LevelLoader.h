////////////////////////////////////////////////////////////////
//
//	LevelLoader.h
//	Loads a level
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H
//
#include "Serialization.h"
//
namespace Wye
{
	class Object;
	class Sprite;
	//
	class LevelLoader: public Object
	{
//#pragma message("Including Class: LevelLoader")
	protected:
		LevelLoader();
	public:
		friend class GameLogic;
		virtual ~LevelLoader();
		void loadLevel(int level);
		//
	private:
		void loadCompositions(ISerializer& stream);
		void loadTextures();
		void loadShaders();
		void loadAssets();
	};
}
//
#endif