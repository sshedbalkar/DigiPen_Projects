#pragma once

#include "Component.h"
#include "material.h"

namespace Framework
{

class Transform;

class VisualPlatform : public GameComponent
{
	Transform* transform;
	MaterialType top;
	MaterialType bottom;
	std::string box;
public:
	VisualPlatform();
	virtual ~VisualPlatform();

	virtual void Serialize( ISerializer& stream );
	virtual void Initialize();
//	virtual void Load();
//	virtual void Unload();
	virtual void Free();



};

}