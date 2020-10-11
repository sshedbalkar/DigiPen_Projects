#include "tile.h"
#include "shader.h"
#include "Transform.h"
#include "Graphics.h"
#include <iostream>

const int MAX_TILE_PIECES		= 1000;
const float SEAM_WIDTH_RATE		= 0.07f;
const float SEAM_DEPTH_RATE		= 0.12f;

namespace Framework
{

//====================================================
Tile::Tile()
{
}
//====================================================
Tile::~Tile()
{
	Free();
}
//====================================================
void Tile::Initialize( ID3D10Device* device )
{
	D3D10_BUFFER_DESC vb_desc;
	ZeroMemory( &vb_desc, sizeof( vb_desc ) );
	vb_desc.Usage = D3D10_USAGE_DYNAMIC;
	vb_desc.ByteWidth = sizeof( TilePieceData ) * MAX_TILE_PIECES;
	vb_desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vb_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vb_desc.MiscFlags = 0;

	device->CreateBuffer( &vb_desc, NULL, &vb );

	reflection_ptr = shader->GetVariableByName("reflection")->AsShaderResource();
}
//====================================================
void Tile::Update( float timeslice )
{
	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		Vec3* cur_pos = &cit->transform->Position;
		Vec3 dpos( cur_pos->x - cit->prev_pos.x, cur_pos->y - cit->prev_pos.y, cur_pos->z - cit->prev_pos.z );
		for ( int i = 0; i < cit->count; ++i )
		{
			cit->pieces[i].position += dpos;
		}
		cit->prev_pos = *cur_pos;
	}

}
//====================================================
void Tile::Draw( ID3D10Device* device, Mat4& mat_view, Mat4& mat_proj, float* reflect_altitude )
{
	unsigned int stride = sizeof( TilePieceData ), offset = 0;
	TilePieceData* mapped;

	ID3D10ShaderResourceView* srv, *prev_srv = NULL;
	shader->SetTransformationConstants( device, mat_view, mat_proj );
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );

	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		if ( !reflect_altitude || ( *reflect_altitude < cit->transform->Position.y ) )
		{
			vb->Map( D3D10_MAP_WRITE_DISCARD, 0, (void**)&mapped );
			GRAPHICS->GetReflection( cit->reflection_index, &srv );
			if ( srv != prev_srv ) { reflection_ptr->SetResource(srv); prev_srv = srv; }
			int count = cit->count;
			memcpy( mapped, cit->pieces, count * sizeof( TilePieceData ) );
			vb->Unmap();
			device->IASetVertexBuffers(0,1,&vb,&stride,&offset );
			shader->Apply( device );
			device->Draw( count, 0 );
		}
	}
}
//====================================================
void Tile::Unload()
{
	ClusterListType::iterator cit;
	for ( cit = cluster_list.begin(); cit != cluster_list.end(); ++cit )
	{
		delete [] cit->pieces;
	}
	cluster_list.clear();
}
//====================================================
void Tile::Free()
{
	Unload();
	S_RELEASE(vb);
}
//====================================================
void Tile::RegisterVolume( Transform* transform, int flag )
{
	ClusterData cluster;
	cluster.transform = transform;
	cluster.prev_pos = transform->Position;
	cluster.pieces = NULL;
	cluster.reflection_index = GRAPHICS->RegisterReflectiveSurface( transform->Position.y + transform->Scale.y * 0.5f );
	cluster_list.push_back( cluster );
	PopulateSurface( cluster_list.back(), 1.5f, flag );
}
//====================================================
void Tile::PopulateSurface( ClusterData& cluster, float density, int flag )
{
//	Surface* surface = cluster.surface;
	float surface_width = cluster.transform->Scale.x;
	float surface_depth = cluster.transform->Scale.z;
	int side_x = static_cast<int>( density * surface_width);
	int side_z = static_cast<int>( density * surface_depth * 0.5f );
	float piece_width = surface_width / static_cast<float>(side_x);
	float piece_depth = surface_depth / static_cast<float>(side_z);
	float seam_width = piece_width * SEAM_WIDTH_RATE;
	float seam_depth = piece_depth * SEAM_DEPTH_RATE;
	piece_width -= seam_width;
	piece_depth -= seam_depth;
	cluster.count = side_x * side_z;
	cluster.pieces = new TilePieceData[cluster.count];
	TilePieceData* pieces = cluster.pieces;
	int n = 0;
	float x = cluster.transform->Position.x - surface_width * 0.5f + piece_width * 0.5f + 0.5f * seam_width;
	float y_factor = (float)flag;
	float y = cluster.transform->Position.y;
	y += cluster.transform->Scale.y * 0.5f * y_factor;

	for ( int i = 0; i < side_x; ++i )
	{
		float z = cluster.transform->Position.z - surface_depth * 0.5f + piece_depth * 0.5f + 0.5f * seam_depth;
		for ( int j = 0; j < side_z; ++j )
		{
			pieces[n].position.x = x;
			pieces[n].position.y = y;
			pieces[n].position.z = z;
			pieces[n].color = Vec4( 1.0f, 0.6f, 0.4f, 1.0f );
			pieces[n].size = Vec2( piece_width, piece_depth );
			float normal_x = GaussianRand( 0.0f, 0.7f );
			float normal_y = GaussianRand( y_factor, 0.2f );
			float normal_z = GaussianRand( 0.0f, 0.7f );
			pieces[n].normal = Vec3( normal_x, normal_y, normal_z );
			D3DXVec3Normalize( &pieces[n].normal, &pieces[n].normal );

			++n;
			z += ( piece_depth + seam_depth );
		}
		x += piece_width + seam_width;
	}
}

}
