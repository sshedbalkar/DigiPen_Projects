#pragma once

#include "VMath.h"
#include "material.h"
#include <list>

namespace Framework
{

class Texture;
	
class TexturedBox : public Material
{
	//
	struct BoxData
	{
		Vec3 position;
		Vec3 size;
	};
	struct ClusterData
	{
		Transform* transform;
		BoxData* box;
		Texture* texture;
	};
	typedef std::list<ClusterData> ClusterListType;
	Texture* texture;

private:
	ClusterListType cluster_list;
	ID3D10Buffer* vb;
	
	ID3D10EffectShaderResourceVariable* diffuse_map_ptr;
	ID3D10EffectShaderResourceVariable* normal_map_ptr;

public:
	TexturedBox();
	~TexturedBox();
	virtual void Initialize( ID3D10Device* device );
	virtual void Update( float timeslice );
	virtual void Draw( ID3D10Device* device, Mat4& mat_view, Mat4& mat_proj, float* reflect_altitude = NULL );
	virtual void RegisterVolume( Transform* transform, int flag );
	virtual void Free();
	virtual void Unload();

	void SetTexture( Texture* _texture ) { texture = _texture; };
private:
	void PopulateBox( ClusterData& cluster, int flag );
};

}