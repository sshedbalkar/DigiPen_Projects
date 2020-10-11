#include "texturedBox.h"
#include "Transform.h"
#include "shader.h"
#include "texture.h"

const int MAX_BOXES = 100;

namespace Framework
{

//===============================================
TexturedBox::TexturedBox() : texture( NULL )
{
}
//===============================================
TexturedBox::~TexturedBox()
{
	Free();
}
//===============================================
void TexturedBox::Initialize( ID3D10Device* device )
{
	D3D10_BUFFER_DESC vb_desc;
	ZeroMemory( &vb_desc, sizeof( vb_desc ) );
	vb_desc.Usage = D3D10_USAGE_DYNAMIC;
	vb_desc.ByteWidth = sizeof( BoxData ) * MAX_BOXES;
	vb_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags = 0;

	device->CreateBuffer( &vb_desc, NULL, &vb );

	diffuse_map_ptr = shader->GetVariableByName("diffuse_map")->AsShaderResource();
	normal_map_ptr = shader->GetVariableByName("normal_map")->AsShaderResource();
}
//===============================================
void TexturedBox::Update( float )
{
	ClusterListType::iterator cit;
	for( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		cit->box->position = cit->transform->Position;
	}
}
//===============================================
void TexturedBox::Draw( ID3D10Device* device, Mat4& mat_view, Mat4& mat_proj, float* reflect_altitude )
{
	
	int vcount = 0;
	unsigned int stride = sizeof( BoxData ), offset = 0;
	BoxData* mapped;

	shader->SetTransformationConstants( device, mat_view, mat_proj );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	vb->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**)&mapped );
	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		if ( !reflect_altitude || *reflect_altitude < cit->transform->Position.y )
		{
			if ( cit->texture ) 
			{
				ID3D10ShaderResourceView* diffuse = cit->texture->GetDiffuse();
				ID3D10ShaderResourceView* normal = cit->texture->GetNormal();
				if ( diffuse ) diffuse_map_ptr->SetResource( diffuse );
				if ( normal ) normal_map_ptr->SetResource( normal );
			}
			memcpy( mapped + vcount, cit->box, sizeof( BoxData ) );
			++vcount;
		}
	}
	vb->Unmap();

	device->IASetVertexBuffers(0,1,&vb,&stride,&offset );
	shader->Apply( device );
	device->Draw( vcount, 0 );
}
//===============================================
void TexturedBox::Unload()
{
	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		delete cit->box;
	}
	cluster_list.clear();
}
//===============================================
void TexturedBox::Free()
{
	Unload();
	S_RELEASE(vb);
}
//===============================================
void TexturedBox::RegisterVolume( Transform* transform, int flag )
{
	ClusterData cluster;
	cluster.transform = transform;
	cluster.texture = texture;
	cluster_list.push_back( cluster );
	PopulateBox( cluster_list.back(), flag );
}
//===============================================
void TexturedBox::PopulateBox( ClusterData& cluster, int flag )
{
	cluster.box = new BoxData;
	cluster.box->position = cluster.transform->Position;
	cluster.box->size = cluster.transform->Scale;
}

}