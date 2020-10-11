#include "states.fxh"
#define TILE_HEIGHT		0.05f
#define SHININESS		30.0f

cbuffer PerFrameConstants
{
	matrix mat_view;
	matrix mat_proj;
	matrix mat_shadow;
};

Texture2D reflection;

//--------------------------------------------------------------------------------------
//VERTEX AND PIXEL SHADER INPUTS
//--------------------------------------------------------------------------------------
struct VSInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 size : TEXCOORD0;		// x = width, y = height
	float2 tilt : TEXCOORD1;		// x = rot tangent, y = rot binormal
};

struct GSInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	float2 size : TEXCOORD0;		// x = width, y = height
	float2 tilt : TEXCOORD1;		// x = rot tangent, y = rot binormal
};

struct PSInputType
{
	float4 position : SV_POSITION;
	float4 reflection_position : TEXCOORD1;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 world_position : WORLD;
};

struct PSOutputType
{
	float4 diffuse : SV_Target0;
	float4 normal_flag: SV_Target1;
	float4 view_position : SV_Target2;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
GSInputType VS( VSInputType vin )
{
	GSInputType vout;
	vout.position = vin.position;
	vout.normal = vin.normal;
	vout.color = vin.color;
	vout.size = vin.size;
	vout.tilt = vin.tilt;
	return vout;
}
//--------------------------------------------------------------------------------------
// GEOMETRY SHADER
//--------------------------------------------------------------------------------------
float4 ProjectionTransform( float3 position )
{
	return mul( float4( position, 1.0f ), mat_proj );
}
float3 ViewingTransform( float3 position )
{
	return mul( float4(position,1.0f), mat_view ).xyz;
}
inline float4 ReflectionTransform( float3 position )
{
	float4 ret = float4( position, 1.0f );
	ret = mul( ret, mat_view );
	ret = mul( ret, mat_proj );
	return ret;
}
[maxvertexcount(16)]
void GS( point GSInputType gin[1], inout TriangleStream<PSInputType> gout )
{
	PSInputType v;
	v.color = gin[0].color;

	float2 offset = gin[0].size / 2.0f;
	float dent = normalize(gin[0].normal.y);
	float3 normal = gin[0].normal;
	float3 world;
	//top side
	v.normal = mul( gin[0].normal, (float3x3)mat_view );

	world = float3( gin[0].position.x - offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	v.reflection_position = ReflectionTransform( world );
	gout.Append(v);
	
	world = float3( gin[0].position.x + offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	v.reflection_position = ReflectionTransform( world );
	gout.Append(v);
	
	world = float3( gin[0].position.x - offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z + offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	v.reflection_position = ReflectionTransform( world );
	gout.Append(v);
	
	world = float3( gin[0].position.x + offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z + offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	v.reflection_position = ReflectionTransform( world );
	gout.Append(v);
	gout.RestartStrip();
	//front side
	v.color.rgb *= 0.5f;
	v.normal = mul( float3( 0.0f, 0.0f, -1.0f ), (float3x3)mat_view );

	world = float3( gin[0].position.x - offset.x, gin[0].position.y, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x + offset.x, gin[0].position.y, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x - offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);

	world = float3( gin[0].position.x + offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	//left side
	v.normal = mul( float3( -1.0f, 0.0f, 0.0f ), (float3x3)mat_view );
	world = float3( gin[0].position.x - offset.x, gin[0].position.y, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x - offset.x, gin[0].position.y, gin[0].position.z + offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x - offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x - offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z + offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	//right side
	v.normal = mul( float3( -1.0f, 0.0f, 0.0f ), (float3x3)mat_view );

	world = float3( gin[0].position.x + offset.x, gin[0].position.y, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x + offset.x, gin[0].position.y, gin[0].position.z + offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x + offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z - offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
	
	world = float3( gin[0].position.x + offset.x, gin[0].position.y + dent * TILE_HEIGHT, gin[0].position.z + offset.y );
	v.world_position = ViewingTransform( world );
	v.position = ProjectionTransform( v.world_position );
	gout.Append(v);
}
//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
PSOutputType PS( PSInputType pin )
{ 
	PSOutputType pout;

	float2 tex = pin.reflection_position.xy;
	tex /= pin.reflection_position.w;
	tex *= 0.5f;
	tex += 0.5f;
	tex = 1.0f - tex;
	float4 reflection_color = reflection.Sample( sample_point_wrap, tex );

	pout.normal_flag.xyz = pin.normal;
	pout.normal_flag.w = 5;
	pout.diffuse = pin.color * reflection_color;
	pout.view_position.xyz = pin.world_position;
	pout.view_position.w = SHININESS;
	return pout;
}
//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 TileShader
{
	pass pass0
	{
		SetBlendState( noblend, float4(0,0,0,0), 0xffffffff );
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( CompileShader( gs_4_0, GS() ) );		
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }

}
