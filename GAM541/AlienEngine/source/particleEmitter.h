#pragma once

#include "Component.h"
#include "VMath.h"
#include <D3D10.h>

namespace Framework
{

enum ParticleType
{
	PT_LAUNCHER = 1,
	PT_GRAVITYAFFECTING = 2,
	PT_SHRINKING = 4,
	PT_ROTATING = 8,
	PT_RANDOMROTATION = 16
	//PT_FADING = 16
};
enum BlendMethod
{
	BLEND_NO_BLEND = 0,
	BLEND_ADD_DIFFUSE = 1,
	BLEND_SUB_DIFFUSE = 2
};

class Transform;
class Texture;

class ParticleEmitter : public GameComponent
{
	friend class Graphics;

	Vec3 offset_bottomleft;
	Vec3 offset_topright;
	Vec3 initial_velocity;
	Vec3 velocity_deviation;
	float life_span;
	float life_span_deviation;
	float particles_per_millisecond;
	float interval;
	float radius;
	Texture* texture;
	int flag;
	unsigned int type;
	int blend_method;
	int max_particles;

	float timer;
	ID3D10Buffer* vb_launcher;
	ID3D10Buffer* vb[2];
	int cur_target_buffer;

	bool initializing;
	bool activated;
	bool deactivating;

	Transform* transform;
public:
	ParticleEmitter();
	~ParticleEmitter();
	
	virtual void Serialize(ISerializer& str);

	virtual void Initialize();
	virtual void Load(){};
	virtual void Unload(){};
	virtual void Free();

	void Activate( bool enable = true );
	
	virtual void SendMessage(Message *){};
};

}