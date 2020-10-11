#pragma once
//
#define MAX_BOIDS 500
namespace Wye
{
	class Flocking
	{
	public:
		Flocking();
		virtual ~Flocking();
		void Init();
		void Run( float dt );
	};
}