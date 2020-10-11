#pragma once

#include "VMath.h"
#include "material.h"
#include <list>

namespace Framework
{
	
class Tile : public Material
{
	//
	struct TilePieceData
	{
		Vec3 position;
		Vec3 normal;
		Vec4 color;
		Vec2 size;
		Vec2 tilt;
	};
	struct ClusterData
	{
		int reflection_index;
		Transform* transform;
		Vec3 prev_pos;
		TilePieceData* pieces;
		int count;
	};
	typedef std::list<ClusterData> ClusterListType;

private:
	ClusterListType cluster_list;
	ID3D10Buffer* vb;
	ID3D10EffectShaderResourceVariable* reflection_ptr;

public:
	Tile();
	~Tile();
	virtual void Initialize( ID3D10Device* device );
	virtual void Update( float timeslice );
	virtual void Draw( ID3D10Device* device, Mat4& mat_view, Mat4& mat_proj, float* reflect_altitude = NULL );
	virtual void RegisterVolume( Transform* transform, int flag );
	virtual void Free();
	virtual void Unload();
private:
	void PopulateSurface( ClusterData& cluster, float density, int flag );
};

}
