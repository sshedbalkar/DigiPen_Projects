#include "Boid.h"
#include "ComponentTypeIds.h"
#include "Core.h"
#include "DebugTools.h"
#include "Serialization.h"
#include "Message.h"
#include "Transform.h"
#include "Physics.h"
#include "Utility.h"
#include "DebugDraw.h"
#include "Graphics.h"
#include "WindowsSystem.h"
//
namespace Wye
{
#define		BOID_MAX_VEL	0.25f
#define		WEIGHT_SEPERATION	0.7f
#define		WEIGHT_ALIGNEMENT	1.2f
#define		WEIGHT_COHESION		1.2f
	//
	float Boid::_boidRadius = 0.0025f;
	float Boid::_boidSeperation = 0.00085f;
	float Boid::_dispSep = 0.0f;
	float Boid::_dispCoh = 0.0f;
	//Vec3 Boid::_imgScale(50.0f, 50.0f, 50.0f );
	const Vec3 Boid::_scale = Vec3(800.0f, 600.0f, 0.0f );
	const Vec3 Boid::_origin = Vec3(-400.0f, -300.0f, 0.0f );
	const float Boid::_velMag = 0.2f;
	bool Boid::_debug = false;
	//
#define EPSI		0.00001f
	//
	Boid::Boid()
		:Component(CT_Boid),
		_dir( 0.0f, 1.0f, 0.0f ),
		_currPos( 0.0f, 0.0f, 0.0f ),
		_frameCount( 0 )
	{
		//
	}
	Boid::~Boid()
	{
		//COLLISION_MAP->clearBit((uint)_currCoord.x, (uint)_currCoord.y, COLLISION_Boid);
		//CORE->unregisterForUpdateMsg(this);
	}
	void Boid::initialize()
	{
		//std::cout<<"Created Boid: parent: "<<(*getParent()).getId()<<"\n";
		_transform = ((*getParent()).getComponent<Transform>(CT_Transform));
		_trPos = &_transform->position;
		_trRot = &_transform->rotation;
		CORE->registerForUpdateMsg(this);
		_currPos = Vec3( randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), 0.0f );
		setNewTarget();
		_framediff = (int)randomFloat( 10.0f, 20.0f );
		float rnd = 0.0015f;
		_delta = Vec3( randomFloat(-rnd, rnd), randomFloat(-rnd, rnd), 0.0f );
		//
	}
	void Boid::serialize(const ISerializer& stream)
	{
		ISerializer* str = stream.clone();
		//str->readProperty("Data", *str);
		//
		//str->readProperty("value", _value);
		//
		delete str;
	}
	void Boid::BoidList( Boid** pBoid )
	{
		Boids = pBoid;
	}
	void Boid::onMessage(const Message& msg)
	{
		if(msg.MessageId == Msgid::EnterFrame)
		{
			const EnterFrameMsg* m = static_cast<const EnterFrameMsg*>(&msg);
			//
			Flock( m->dt );
			_currPos += _vel * _velMag * m->dt;
			Wrap();
			setCurrPos();
			if(_debug)
			{
				float r1 = sqrtf(_boidRadius) * _scale.x * 0.5f;
				Vec4 green(0.3f, 0.8f, 0.2f, 0.9f);
				Drawer::Instance.setColor(green);
				Drawer::Instance.drawCircle2D(*_trPos, r1, 20);
				//
				float r2 = sqrtf(_boidSeperation) * _scale.x * 0.5f;
				Vec4 red(0.8f, 0.3f, 0.2f, 0.9f);
				Drawer::Instance.setColor(red);
				Drawer::Instance.drawCircle2D(*_trPos, r2, 20);
				//
				DrawVecFromCurr( _vel, Vec4(1.0f, 1.0f, 1.0f, 0.8f), m->dt );
				//DrawVecFromCurr( _seperation, Vec4(0.8f, 0.1f, 0.1f, 0.9f), m->dt );
				DrawVecFromCurr( _align, Vec4(0.1f, 0.9f, 0.1f, 0.9f), m->dt );
				DrawVecFromCurr( _cohesion, Vec4(0.1f, 0.1f, 0.9f, 0.9f), m->dt );
				//
				float len = sqrtf(lengthSquared(_seperation));
				//std::cout<<"Len: "<<len<<"\n";
			}
		}
		else if(msg.MessageId == Msgid::ToggleDebugInfo)
		{
			const ToggleDebugDisplay* m = static_cast<const ToggleDebugDisplay*>(&msg);
			_debug = m->debugActive;
		}
	}
	void Boid::UpdateDispParams()
	{
		_dispSep = sqrtf(_boidSeperation) * _scale.x * 0.5f;
		_dispCoh = sqrtf(_boidRadius) * _scale.x * 0.5f;
	}
	void Boid::DrawInfo()
	{
		char str[100];
		//
		sprintf_s(str, "Cohesion: %0.3f", _dispCoh);
		GRAPHICS->drawText( str, 12, 1, Vec4(1, 1, 1, 1) );
		//
		sprintf_s(str, "Seperation: %0.3f", _dispSep);
		GRAPHICS->drawText( str, 26, 1, Vec4(1, 1, 1, 1) );
	}
	void Boid::DrawVecFromCurr( Vec3 vec, const Vec4& color, const float& dt )
	{
		vec *= _scale.x * dt * 5;
		vec += *_trPos;
		Drawer::Instance.moveTo(*_trPos);
		Drawer::Instance.setColor(color);
		Drawer::Instance.lineTo(vec);
	}
	void Boid::Wrap()
	{
	/*	
		if( _currPos. x < -1.0f)
		{
			_currPos. x = 1.0f;
			//_vel.x *= -1.0f;
		}
		else if(_currPos. x > 1.0f)
		{
			_currPos. x = -1.0f;
			//_vel.x *= -1.0f;
		}
		if( _currPos. y < -1.0f)
		{
			_currPos. y = 1.0f;
			//_vel.y *= -1.0f;
		}
		else if(_currPos. y > 1.0f)
		{
			_currPos. y = -1.0f;
			//_vel.y *= -1.0f;
		}
		*/
		if( _currPos. x < -1.1f || _currPos. x > 1.1f ) _currPos. x *= -1.0f;
		if( _currPos. y < -1.1f || _currPos. y > 1.1f ) _currPos. y *= -1.0f;
	}
	void Boid::Flock( const float &dt )
	{
		CalculateNeighbors();
		Vec3 accel( 0.0f, 0.0f, 0.0f );
		_seperation = Seperation();
		_align = Alignment();
		_cohesion = Cohesion();
		accel += _seperation;
		accel += _align;
		accel += _cohesion;
		//
		//std::cout<<"Accel: "<<accel<<"\n";
		_vel += accel;
		if( ++_frameCount == _framediff )
		{
			float rnd = 0.0015f;
			_delta = Vec3( randomFloat(-rnd, rnd), randomFloat(-rnd, rnd), 0.0f );
			_frameCount = 0;
			_framediff = (int)randomFloat( 10.0f, 20.0f );
		}
		else
		{
			_vel += _delta;
		}
		float len = lengthSquared( _vel );
		if( len > BOID_MAX_VEL )
		{
			normalize( _vel );
			_vel *= BOID_MAX_VEL;
		}
	}
	Vec3 Boid::Seperation()
	{
		Vec3 seperation( 0.0f, 0.0f, 0.0f );
		int count = 0;
		Boid* boid;
		float dist;
		Vec3 diff;
		//
		for(int i = 0; i < _neighborCount; ++i )
		{
			boid = _neighbors[i];
			diff = _currPos - boid->_currPos;
			//st = sqrtf(lengthSquared( diff ));
			dist = lengthSquared( diff );
			//std::cout<<"dist: "<< dist<<"\n";
			if( dist > 0.0f && dist < _boidSeperation )
			{
				normalize( diff );
				diff /= sqrtf(dist);
				seperation += diff;
				++count;
			}
		}
		if( count > 0 )
		{
			seperation /= (float)count;
		}
		return seperation * WEIGHT_SEPERATION;
	}
	Vec3 Boid::Cohesion()
	{
		Vec3 target( 0.0f, 0.0f, 0.0f );
		int count;
		Boid* boid;
		float dist;
		Vec3 diff;
		for( int i = 0; i < _neighborCount; ++i )
		{
			boid = _neighbors[i];
			diff = boid->_currPos - _currPos;
			dist = lengthSquared( diff );
			if( dist > 0.0f && dist < _boidRadius )
			{
				target += boid->_currPos;
				++count;
			}
		}
		if( count > 0 )
		{
			float rnd = 0.0015f;
			target += Vec3( randomFloat(-rnd, rnd), randomFloat(-rnd, rnd), 0.0f );
			target /= (float)count;
			target = Steer( target );
		}
		return target * WEIGHT_COHESION;
	}
	Vec3 Boid::Steer( Vec3 &target )
	{
		Vec3 desired = target - _currPos;
		float len;
		float dist = lengthSquared( desired );
		if( dist > 0.0f )
		{
			normalize( desired );
			if( dist < 100.0f )
			{
				desired *= BOID_MAX_VEL * dist / 100.0f;
			}
			else
			{
				desired *= BOID_MAX_VEL;
			}
			desired -= _vel;
			len = lengthSquared( desired );
			if( len > BOID_MAX_VEL )
			{
				normalize( desired );
				desired *= BOID_MAX_VEL;
			}
			return desired;
		}
		return Vec3( 0.0f, 0.0f, 0.0f );
	}
	Vec3 Boid::Alignment()
	{
		Vec3 align( 0.0f, 0.0f, 0.0f );
		int count = 0;
		Boid* boid;
		float dist;
		Vec3 diff;
		for( int i = 0; i < _neighborCount; ++i )
		{
			boid = _neighbors[i];
			diff = boid->_currPos - _currPos;
			dist = lengthSquared( diff );
			if( dist > 0.0f && dist < _boidRadius )
			{
				align += boid->_vel;
				++count;
			}
		}
		if( count > 0 )
		{
			align /= (float)count;
			if( lengthSquared(align) > BOID_MAX_VEL )
			{
				normalize( align );
				align *= BOID_MAX_VEL;
			}
		}
		return align * WEIGHT_ALIGNEMENT;
	}
	void Boid::CalculateNeighbors()
	{
		_neighborCount = 0;
		Vec3 diff;
		Boid *boid;
		for( int i = 0; i < MAX_BOIDS; ++i )
		{
			boid = Boids[i];
			if( this == boid ) continue;
			diff = _currPos - boid->_currPos;
			if( lengthSquared(diff) < _boidRadius )
			{
				_neighbors[_neighborCount] = Boids[i];
				++_neighborCount;
			}
		}
	}
	void Boid::setCurrPos()
	{
		//_transform->position = (_currPos * _scale) + _origin;
		Vec3 temp = *_trPos;
		_trPos->x = (_currPos.x * _scale.x) + _origin.x;
		_trPos->y = (_currPos.y * _scale.y) + _origin.y;
		//_trPos->z = (_currPos.z * _scale.z) + _origin.z;
		_trPos->z = 0.0f;
		//
		/*
		float r = sqrt((_vel.x * _vel.x) + (_vel.y * _vel.y) + (_vel.z * _vel.z));
		//_trRot->z = atan2( _vel.y, _vel.x );
		//_trRot->z = atan( _vel.y/_vel.x );
		//_trRot->y = atan2( _vel.z, r );
		//_trRot->y = acos( _vel.z/r );
		//_trRot->x = 0.0f;
		Vec3 v = _vel;
		normalize( v );
		Vec2 x, y, z;
		_trRot->x = dot( Vec2(_vel.x, -_vel.y), Vec2(0, 1) );
		_trRot->y = dot( Vec2(_vel.x, _vel.z), Vec2(0, 0) );
		_trRot->z = dot( Vec2(_vel.y, _vel.z), Vec2(1, 0) );
		//double yaw = Math.Atan2(ds.X, ds.Y);
		//double pitch = Math.Atan2(ds.Z, Math.Sqrt((ds.X * ds.X) + (ds.Y * ds.Y)));
		//std::cout<<"Old: "<<temp<<", New: "<<(*_trPos)<<"\n";
		*/
		_trRot->z = atan2( _vel.y, _vel.x );
	}
	void Boid::setNewTarget()
	{
		//std::cout<<"Set new target\n";
		_targetPos = Vec3( randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), 0.0f );
		//_targetPos = Vec3( 10.0f, 10.0f, 0.0f );
		_vel = ( _targetPos - _currPos ) * 2.5f;
	}
}
