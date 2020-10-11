#include "visualPlatform.h"
#include "Composition.h"
#include "Transform.h"
#include "Graphics.h"
#include "texturedbox.h"
#include <iostream>

namespace Framework
{

//===================================================
VisualPlatform::VisualPlatform()
{
}
//===================================================
VisualPlatform::~VisualPlatform()
{
}
//===================================================
void VisualPlatform::Serialize( ISerializer& stream )
{
	ISerializer* stream_platform = stream.clone();
	std::string str;
	stream_platform->readValue( "Box", box );
	stream_platform->readValue( "Top", str );
	top = Material::GetMaterialTypeFromString( str );
	stream_platform->readValue( "Bottom", str );
	bottom = Material::GetMaterialTypeFromString( str );

	delete stream_platform;
}
//===================================================
void VisualPlatform::Initialize()
{
	transform = GetOwner()->has( Transform );
	if ( !transform ) return;
	
	TexturedBox* box_material = (TexturedBox*)GRAPHICS->GetMaterial( MAT_TEXTUREDBOX );
	if ( box_material )
	{
		Texture* texture = GRAPHICS->GetTexture( box );
		box_material->SetTexture( texture );
		box_material->RegisterVolume( transform, 0 );
	}

	Material* top_material = GRAPHICS->GetMaterial( top );
	if ( top_material )
		top_material->RegisterVolume( transform, 1 );
	
	Material* bottom_material = GRAPHICS->GetMaterial( bottom );
	if ( bottom_material )
		bottom_material->RegisterVolume( transform, -1 );
}
//===================================================
void VisualPlatform::Free()
{
}

}//end namespace