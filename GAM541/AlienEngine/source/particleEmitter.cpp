#include "particleEmitter.h"
#include "Composition.h"
#include "Graphics.h"

namespace Framework
{

//=====================================================
ParticleEmitter::ParticleEmitter() : initializing(true), deactivating(false), activated(false), texture( NULL )
{
}
//=====================================================
ParticleEmitter::~ParticleEmitter()
{
	Free();
}
//=====================================================
void ParticleEmitter::Serialize( ISerializer& stream )
{
	ISerializer* emitter = stream.clone();
	std::string str;
	int k;

	if ( !emitter->readValue( "MaxParticles", max_particles ) )
		max_particles = 1000;
	if ( !emitter->readValue( "BottomLeftFront", offset_bottomleft ) ) 
		offset_bottomleft = Vec3( 0.0f,0.0f,0.0f );
	if ( !emitter->readValue( "TopRightBack", offset_topright ) )
		offset_topright = Vec3( 0.0f, 0.0f, 0.0f );
	if ( !emitter->readValue( "InitialVelocity", initial_velocity ) )
		initial_velocity = Vec3( 0.0f, 0.0f, 0.0f );
	if ( !emitter->readValue( "VelocityDeviation", velocity_deviation ) )
		velocity_deviation = Vec3( 0.0f, 0.0f, 0.0f );
	if ( !emitter->readValue( "LifeSpan", life_span ) )
		life_span = 1.0f;
	if ( !emitter->readValue( "LifeSpanDeviation", life_span_deviation ) )
		life_span_deviation = 0.0f;
	if ( !emitter->readValue( "ParticlesPerMillisecond", particles_per_millisecond ) )
		particles_per_millisecond = 1.0f;
	if ( !emitter->readValue( "Interval", interval ) )
		interval = 0.0f;
	if ( !emitter->readValue( "ParticleRadius", radius ) )
		radius = 0.1f;
	if ( emitter->readValue( "Texture", str ) )
		texture = GRAPHICS->GetTexture( str );
	if ( emitter->readValue( "BlendMethod", str ) )
	{
		if ( str == "Add" ) blend_method = BLEND_ADD_DIFFUSE;
		else if ( str == "Inv" ) blend_method = BLEND_SUB_DIFFUSE;
		else blend_method = BLEND_NO_BLEND;
	}
	else blend_method = BLEND_NO_BLEND;

	type = 0;
	k = 0;
	emitter->readValue( "RandomRotation", k );
	if ( k == 1 ) type |= PT_RANDOMROTATION;
	k = 0;
	emitter->readValue( "GravityAffecting", k );
	if ( k == 1 ) type |= PT_GRAVITYAFFECTING;
	k = 0;
	emitter->readValue( "Shrinking", k );
	if ( k == 1 ) type |= PT_SHRINKING;
	k = 0;
	emitter->readValue( "Rotating", k );
	if ( k == 1 ) type |= PT_ROTATING;

	if ( !emitter->readValue( "FilterFlag", flag ) )
		flag = 0;

	//initial_velocity = Vec3( 2.0f, 1.0f, 0.0f );
	GRAPHICS->RegisterParticleEmitter( this );
	delete emitter;
}
//=====================================================
void ParticleEmitter::Initialize()
{
	transform = GetOwner()->has(Transform);
}
//=====================================================
void ParticleEmitter::Free()
{
	S_RELEASE(vb[0]);
	S_RELEASE(vb[1]);
}
//=====================================================
void ParticleEmitter::Activate( bool enable )
{
	if ( enable )
	{
		activated = true;
		deactivating = false;
	}
	else
	{
		deactivating = true;
		timer = life_span + life_span_deviation;
	}
}

}