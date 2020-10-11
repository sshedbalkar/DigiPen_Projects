////////////////////////////////////////////////////////////////
//
//	CollisionMap.h
//	Binary collision map
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef COLLISION_MAP_H
#define COLLISION_MAP_H
//
#include <sstream>
//
namespace Wye
{
#define COLLISION_INVALID	0x00000000
#define	COLLISION_LEFT		0x00000001	//0001
#define	COLLISION_RIGHT		0x00000002	//0010
#define	COLLISION_TOP		0x00000004	//0100
#define	COLLISION_BOTTOM	0x00000008	//1000
//
#define COLLISION_EMPTY		0x00000010
#define COLLISION_HERO		0x00000020
#define COLLISION_AI_1		0x00000040
#define COLLISION_BLOCK_1	0x00000080
#define COLLISION_WALL		0x00000100
#define COLLISION_GOLD		0x00000200
#define COLLISION_KEY		0x00000400

	//
	class CollisionMap
	{
#pragma message("Including class: CollisionMap")
	public:
		CollisionMap();
		~CollisionMap();
		int value(unsigned x, unsigned y)const;
		void setBit(unsigned x, unsigned y, unsigned type);
		void clearBit(unsigned x, unsigned y, unsigned type);
		void toggleBit(unsigned x, unsigned y, unsigned type);
		bool testBit(unsigned x, unsigned y, unsigned type)const;
		std::ostream& print(std::ostream&)const;
		void create(unsigned width, unsigned height, int val=0);
		int checkMapCollision(float PosX, float PosY, unsigned collisionFlag, float scaleX, float scaleY);
		//
	private:
		void deallocate();
		int *_map;
		unsigned _width;
		unsigned _height;
	};
	//
	extern CollisionMap* COLLISION_MAP;
}
//
#endif