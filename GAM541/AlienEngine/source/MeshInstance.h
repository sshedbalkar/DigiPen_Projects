#pragma once

#include "Component.h"

namespace Framework
{
class Mesh;
class Texture;
class Animation;
class Transform;

class MeshInstance : public GameComponent
{
public:
	float dt;
	bool looping;
	Transform* transform;
	Animation* animation;
	Texture* texture;
	unsigned int flag;

public:
	MeshInstance();
	virtual ~MeshInstance();

	virtual void Serialize( ISerializer& stream );
	virtual void Initialize();
	virtual void Load();
	virtual void Unload();
	virtual void Free();

	void SetAnimation( std::string& id, bool _looping );
	void SetTexture( std::string& id );

};

}
