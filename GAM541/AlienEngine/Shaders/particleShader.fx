#include "states.fxh"

#define BOX_CURVATURE		0.05f

cbuffer PerFrameConstants
{
	matrix mat_view;
	matrix mat_proj;
};

int flag;

Texture2D diffuse;
Texture2D mask;
//--------------------------------------------------------------------------------------
//VERTEX AND PIXEL SHADER INPUTS
//--------------------------------------------------------------------------------------
struct VSInputType
{
	float3 position : POSITION0;
	float3 velocity : POSITION1;
	float3 v2_timer : POSITION2;
	uint type : VFLAG;
};

struct GSInputType
{
	float3 position : POSITION0;
	float3 v2_timer : POSITION1;
	uint type : VFLAG;
};

struct PSInputType
{
	float4 position : SV_POSITION;
	float3 view_position : POSITION;
	float2 tex : TEXCOORD;
	uint type : VFLAG;
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
	vout.v2_timer = vin.v2_timer;
	vout.type = vin.type;
	return vout;
}
//--------------------------------------------------------------------------------------
// GEOMETRY SHADER
//--------------------------------------------------------------------------------------
[maxvertexcount(4)]
void GS( point GSInputType gin[1], inout TriangleStream<PSInputType> gout )
{
	if ( ( gin[0].type & PT_LAUNCHER) == PT_LAUNCHER ) return;
	matrix mat_translation = {{1,0,0,0},
							  {0,1,0,0},
							  {0,0,1,0},
							  {gin[0].position.x,gin[0].position.y,gin[0].position.z,1}};
	matrix mat_worldview = mul( mat_translation, mat_view );

	float c = cos(gin[0].v2_timer.x);
	float s = sin(gin[0].v2_timer.x);
	matrix mat_rot = {{c,-s,0,0},{s,c,0,0},{0,0,1,0},{0,0,0,1}};
	mat_worldview = mul( mat_rot, mat_worldview );

	PSInputType v;
	float3 position;
	float radius = gin[0].v2_timer.y;
	
	position = float3( -radius.xx, 0.0f );
	v.view_position = mul( float4(position,1.0f), mat_worldview );
	v.position = mul( float4(v.view_position,1.0f), mat_proj );
	v.tex = float2( 0.0f, 1.0f );
	v.type = gin[0].type;
	gout.Append(v);

	position.x = radius;
	v.view_position = mul( float4(position,1.0f), mat_worldview );
	v.position = mul( float4(v.view_position,1.0f), mat_proj );
	v.tex = float2( 1.0f, 1.0f );
	v.type = gin[0].type;
	gout.Append(v);
	
	position.x = -radius;
	position.y = radius;
	v.view_position = mul( float4(position,1.0f), mat_worldview );
	v.position = mul( float4(v.view_position,1.0f), mat_proj );
	v.tex = float2( 0.0f, 0.0f );
	v.type = gin[0].type;
	gout.Append(v);
	
	position.x = radius;
	v.view_position = mul( float4(position,1.0f), mat_worldview );
	v.position = mul( float4(v.view_position,1.0f), mat_proj );
	v.tex = float2( 1.0f, 0.0f );
	v.type = gin[0].type;
	gout.Append(v);
}
//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
PSOutputType PS( PSInputType pin )
{ 
	PSOutputType pout;

	float3 m = mask.SampleLevel( sample_point_wrap, pin.tex, 0 );
	pout.diffuse.xyz = m * diffuse.SampleLevel( sample_point_wrap, pin.tex, 0 ).xyz;
//	pout.diffuse.a = 0.1f;
	pout.view_position.xyz = pin.view_position;
	pout.view_position.w = 0;
	pout.normal_flag.w = (float)flag;
	return pout;
}
//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 ParticleShader
{
	pass pass0
	{
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( CompileShader( gs_4_0, GS() ) );		
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }

}
