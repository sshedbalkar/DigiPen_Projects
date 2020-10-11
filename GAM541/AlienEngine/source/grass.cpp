#include "grass.h"
#include "shader.h"
#include "Transform.h"
#include <iostream>

const int MAX_GRASS_BLADES =		4000;
const float COLOR_FALLOFF  =		0.02f;
const float WAVING_ACCELERATION =	0.4f;
const float GRASS_BLADE_WIDTH =		0.1f;
const float GRASS_BLADE_HEIGHT =	0.8f;

namespace Framework
{
//==============================================
Grass::Grass()
{
}
//==============================================
Grass::~Grass()
{
	Free();
}
//==============================================
void Grass::Initialize( ID3D10Device* device )
{
	D3D10_BUFFER_DESC vb_desc;
	ZeroMemory( &vb_desc, sizeof( vb_desc ) );
	vb_desc.Usage = D3D10_USAGE_DYNAMIC;
	vb_desc.ByteWidth = sizeof( GrassBladeData ) * MAX_GRASS_BLADES;
	vb_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags = 0;

	device->CreateBuffer( &vb_desc, NULL, &vb );
}
//==============================================
void Grass::Update( float timeslice )
{
	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		if ( cit->bend < 0 ) cit->waving_speed += WAVING_ACCELERATION * timeslice;
		else cit->waving_speed -= WAVING_ACCELERATION * timeslice;
		float db = cit->waving_speed * timeslice;
		cit->bend += db;
		Vec3* cur_pos = &cit->transform->Position;
		Vec3 dpos( cur_pos->x - cit->prev_pos.x, cur_pos->y - cit->prev_pos.y, cur_pos->z - cit->prev_pos.z );
		for ( int i = 0; i < cit->count; ++i )
		{
			cit->blades[i].position += dpos;
			cit->blades[i].extra.z += db;
		}
		cit->prev_pos = *cur_pos;
	}
}
//==============================================
void Grass::Draw( ID3D10Device* device, Mat4& mat_view, Mat4& mat_proj, float* reflect_altitude )
{
	int vcount = 0;
	unsigned int stride = sizeof( GrassBladeData ), offset = 0;
	GrassBladeData* mapped;

	shader->SetTransformationConstants( device, mat_view, mat_proj );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	vb->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**)&mapped );
	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		int count = cit->count;
		if ( count + vcount > MAX_GRASS_BLADES ) {printf("grass blades overflowed."); count = MAX_GRASS_BLADES - vcount; }
		memcpy( mapped + vcount, cit->blades, count * sizeof( GrassBladeData ) );
		vcount += count;
	}
	vb->Unmap();

	device->IASetVertexBuffers(0,1,&vb,&stride,&offset );
//	if ( vcount > MAX_GRASS_BLADES ) { printf("grass blades overflowed."); vcount = MAX_GRASS_BLADES; }
	shader->Apply( device );
	device->Draw( vcount, 0 );
}
//==============================================
void Grass::Unload()
{
	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		delete [] cit->blades;
	}
	cluster_list.clear();
}
//==============================================
void Grass::Free()
{
	Unload();
	S_RELEASE(vb);
}
//==============================================
void Grass::RegisterVolume( Transform* transform, int flag )
{
	ClusterData cluster;
	cluster.transform = transform;
	cluster.prev_pos = transform->Position;
	cluster.blades = NULL;
	cluster_list.push_back( cluster );
	PopulateSurface( cluster_list.back(), 6.5f, flag );
}
//==============================================
void Grass::PopulateSurface( ClusterData& cluster, float density, int flag )
{
	Transform* transform = cluster.transform;
	GrassBladeData** pblades = &cluster.blades;
	int count = static_cast<int>(( transform->Scale.x * transform->Scale.z ) * density);
	cluster.count = count;
	cluster.waving_speed = 0.0f;
	cluster.bend = 0.1f;
	*pblades = new GrassBladeData[count];
	GrassBladeData* blades = *pblades;
	float half_width = transform->Scale.x;
	float half_depth = transform->Scale.z;
	half_width *= 0.5f;
	half_width -= GRASS_BLADE_WIDTH;
	half_depth *= 0.5f;
	float y_factor = (float)flag;
	float y = transform->Position.y;
	y += transform->Scale.y * 0.5f * y_factor;

	float n = transform->Scale.z;
	n /= 0.3f;
	n = sqrt(n);
	float step_z = 0.25f;
	float z = transform->Position.z - half_depth;
	int blades_per_layer = (int) ((float)(count) / n );
	++ blades_per_layer;
	int c = 0;

	for ( int i = 0; i < count; ++i )
	{
		if ( c % blades_per_layer == 0 ) { z += step_z; step_z += 0.4f; c = 1; }
		float x = Rand( transform->Position.x - half_width, transform->Position.x + half_width );

		float f = GaussianRand( 0.0f, 0.2f );
		float yellowness = Rand( 0.0f, 0.4f );
		blades[i].position = Vec3( x, y, z );
		blades[i].extra = Vec3( GRASS_BLADE_WIDTH, y_factor * GRASS_BLADE_HEIGHT, cluster.bend + f );
		blades[i].color = Vec4( yellowness, Rand(0.8f, 1.0f ) - COLOR_FALLOFF * (z+half_depth*0.5f),0.2f,1.0f );
		++c;
	}
}

}//end namespace