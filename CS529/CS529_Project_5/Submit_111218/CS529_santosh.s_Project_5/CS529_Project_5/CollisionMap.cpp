#include "CollisionMap.h"
#include "DebugTools.h"
#include "Precompiled.h"
#include "Utility.h"
#include <iomanip>
//#include <iostream>
//
namespace Wye
{
	CollisionMap* COLLISION_MAP = NULL;
	//
	CollisionMap::CollisionMap()
		:_map(0),
		_width(0),
		_height(0)
	{
		ErrorIf(COLLISION_MAP!=NULL,"Gamedata already initialized\n");
		COLLISION_MAP = this;
	}
	CollisionMap::~CollisionMap()
	{
		deallocate();
	}
	int CollisionMap::value(unsigned x, unsigned y)const
	{
		if(x < _width && y < _height)
		{
			return *(_map + (y * _width) + x);
		}
		return COLLISION_INVALID;
	}
	void CollisionMap::setBit(unsigned x, unsigned y, unsigned type)
	{
		if(x < _width && y < _height)
		{
			*(_map + (y * _width) + x) |= type;
		}
	}
	void CollisionMap::clearBit(unsigned x, unsigned y, unsigned type)
	{
		if(x < _width && y < _height)
		{
			*(_map + (y * _width) + x) &= ~type;
		}
	}
	void CollisionMap::toggleBit(unsigned x, unsigned y, unsigned type)
	{
		if(x < _width && y < _height)
		{
			*(_map + (y * _width) + x) ^= type;
		}
	}
	bool CollisionMap::testBit(unsigned x, unsigned y, unsigned type)const
	{
		if(x < _width && y < _height)
		{
			return *(_map + (y * _width) + x) & type;
		}
		return false;
	}
	std::ostream& CollisionMap::print(std::ostream& out)const
	{
		out<<"COLLISION MAP:\n";
		int v = 0;
		for(int i = 0; i < _height; ++i)
		{
			out<<"[";
			for(int j = 0; j < _width; ++j)
			{
				out<<std::setw(4)<<*(_map+v)<<", ";
				++v;
			}
			out<<"]\n";
		}
		return out;
	}
	void CollisionMap::create(unsigned width, unsigned height, int val)
	{
		std::cout<<"Creating map of: "<<width<<"x"<<height<<"\n";
		deallocate();
		//
		_width = width;
		_height = height;
		_map = new int[width * height];
		//
		for(int i = width * height - 1; i > -1; --i)
		{
			*(_map + i) = val;
		}
	}
	void CollisionMap::deallocate()
	{
		safeDelete(_map);
		_width = _height = 0;
	}
	int CollisionMap::checkMapCollision(float PosX, float PosY, unsigned collisionFlag, float scaleX, float scaleY)
	{
		std::cout<<"check collision: x: " <<PosX<<", y: "<<PosY<<", flag: " <<collisionFlag<<"\n";
		int t1, t2, b1, b2, l1, l2, r1, r2;
		int flag = 0;
		//
		t1 = (int) floor(PosY - (scaleY / 2.0f));
		t2 = (int) floor(PosY - (scaleY / 4.0f));
		//
		b1 = (int) floor(PosY + (scaleY / 2.0f));
		b2 = (int) floor(PosY + (scaleY / 4.0f));
		//
		l1 = (int) floor(PosX - (scaleX / 2.0f));
		l2 = (int) floor(PosX - (scaleX / 4.0f));
		//
		r1 = (int) floor(PosX + (scaleX / 2.0f));
		r2 = (int) floor(PosX + (scaleX / 4.0f));
		//
		std::cout<<"t1: "<<t1<<", t2: "<<t2<<", b1: "<<b1<<", b2: "<<b2<<", l1: "<<l1<<", l2: "<<l2<<", r1: "<<r1<<", r2: "<<r2<<"\n";
		if(testBit(l2, t1, collisionFlag) || testBit(r2, t1, collisionFlag))
		{
			flag |= COLLISION_TOP;
			//flag |= COLLISION_BOTTOM;
		}
		if(testBit(l2, b1, collisionFlag) || testBit(r2, b1, collisionFlag))
		{
			flag |= COLLISION_BOTTOM;
			//flag |= COLLISION_TOP;
		}
		if(testBit(l1, t2, collisionFlag) || testBit(l1, b2, collisionFlag))
		{
			flag |=  COLLISION_LEFT;
		}
		if(testBit(r1, t2, collisionFlag) || testBit(r1, b2, collisionFlag))
		{
			flag |= COLLISION_RIGHT;
		}

		return flag;
	}
}