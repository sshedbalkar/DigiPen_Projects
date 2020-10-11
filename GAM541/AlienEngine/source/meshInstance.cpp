#include "MeshInstance.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Composition.h"
#include <iostream>

namespace Framework
{

//===================================================
MeshInstance::MeshInstance() : dt(0.0f), looping(false), transform(NULL), texture(NULL), animation(NULL)
{
}
//===================================================
MeshInstance::~MeshInstance()
{
}
//===================================================
void MeshInstance::Serialize( ISerializer& stream )
{
	std::string str;
	stream.readValue("id", str );

	Mesh* mesh = GRAPHICS->GetMesh( str );
	mesh->AddInstance( this );
}
//===================================================
void MeshInstance::Initialize()
{
	dt = 0.0f;
	transform = GetOwner()->has(Transform);
	std::string id = "WoodTile";
	texture = GRAPHICS->GetTexture(id);
}
//===================================================
void MeshInstance::Load()
{
	dt = 0.0f;
}
//===================================================
void MeshInstance::Unload()
{
}
//===================================================
void MeshInstance::Free()
{
}
//===================================================
void MeshInstance::SetAnimation( std::string& id, bool _looping )
{

}
//===================================================
void MeshInstance::SetTexture( std::string& id )
{
	texture = GRAPHICS->GetTexture( id );
}

}