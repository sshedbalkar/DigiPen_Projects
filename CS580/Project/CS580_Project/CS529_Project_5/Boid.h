////////////////////////////////////////////////////////////////
//
//	Gold.h
//	implements the Gold coin logic
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef BOID_H
#define BOID_H
//
#include "Component.h"
#include "VMath.h"
#include "Flocking.h"
//
namespace Wye
{
	class Sprite;
	class Transform;
	//
	class Boid: public Component
	{
	public:
		Boid();
		~Boid();
		void serialize(const ISerializer& stream);
		void initialize();
		void onMessage(const Message& msg);
		void update(float dt);
		void BoidList( Boid** pBoid );
		static void DrawInfo();
		static void UpdateDispParams();
		//
	public:
		static float	_boidRadius;
		static float	_boidSeperation;
		//
	private:
		void setCurrPos();
		void setNewTarget();
		void CalculateNeighbors();
		void Flock( const float &dt );
		Vec3 Seperation();
		Vec3 Cohesion();
		Vec3 Alignment();
		Vec3 Steer( Vec3 &target );
		void Wrap();
		void DrawVecFromCurr( Vec3 vec, const Vec4& color, const float& dt );
		//
	private:
		//Sprite* _display;
		Transform*		_transform;
		Vec3			_targetPos;
		Vec3			_dir;
		Vec3			_currPos;
		Vec3			_vel;
		Vec3*			_trPos;
		Vec3*			_trRot;
		static const Vec3		_scale;
		static const Vec3		_origin;
		static const float		_velMag;
		Boid**			Boids;
		Boid*			_neighbors[MAX_BOIDS];
		int				_neighborCount;
		static bool		_debug;
		Vec3			_seperation;
		Vec3			_align;
		Vec3			_cohesion;
		Vec3			_delta;
		int				_framediff;
		int				_frameCount;
		static float	_dispSep;
		static float	_dispCoh;
	};
}
//
#endif