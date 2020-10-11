#pragma once

#include <D3D10.h>
#include "VMath.h"
#include <string>

namespace Framework
{

enum MaterialType
{
	MAT_NONE = -1,
	//surface materials
	MAT_GRASS = 0,
	MAT_TILE,
	//box materials
	MAT_TEXTUREDBOX,
	MAX_MATERIALS
};

class Shader;
class Transform;

class Material
{
protected:
	Shader* shader;
public:
	virtual ~Material() {}
	virtual void Initialize( ID3D10Device* device ) = 0;
	virtual void Update( float timeslice ) {};
	virtual void Draw( ID3D10Device* device, Mat4& mat_view, Mat4& mat_proj, float* reflect_altitude = NULL ) = 0;
	virtual void RegisterVolume( Transform* transform, int flag ) = 0;
	virtual void Free() = 0;
	virtual void Unload() = 0;
	void SetShader( Shader* _shader ) { shader = _shader; }

	static MaterialType GetMaterialTypeFromString( std::string& str )
	{
		if ( str == "none" )
		{ return MAT_NONE;}
		else if ( str == "grass" )
		{ return MAT_GRASS;}
		else if ( str == "tile" )
		{ return MAT_TILE;}
		else return MAT_NONE;
	}
};

}
