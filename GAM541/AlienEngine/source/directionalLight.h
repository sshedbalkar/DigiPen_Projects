#pragma once

#include "Component.h"
#include "VMath.h"

namespace Framework
{

class DirectionalLight : public GameComponent
{
public:
	Vec3 direction;
	Vec3 intensity;
	Mat4 mat_shadow_view;

public:
	virtual void Serialize(ISerializer& stream);
	void InitTest();

	void SetMatrix( Mat4& mat );
};

}
