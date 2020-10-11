#include "directionalLight.h"
#include "Graphics.h"

namespace Framework
{

//================================================
void DirectionalLight::Serialize( ISerializer& stream )
{
	direction[0] = 0.0f; direction[1] = 1.0f; direction[2] = 0.0f;
	intensity[0] = 1.0f; intensity[1] = 1.0f; intensity[2] = 1.0f;
	GRAPHICS->SetPrimaryLight( this );
}
void DirectionalLight::InitTest()
{
	direction[0] = 0.0f; direction[1] = 1.0f; direction[2] = 0.0f;
	intensity[0] = 1.0f; intensity[1] = 1.0f; intensity[2] = 1.0f;
	GRAPHICS->SetPrimaryLight( this );
}
}
