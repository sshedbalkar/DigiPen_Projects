#include "states.fxh"

#define BOX_CURVATURE		0.05f

cbuffer PerFrameConstants
{
	matrix mat_view;
	matrix mat_proj;
};

Texture2D diffuse_map;
Texture2D normal_map;

//--------------------------------------------------------------------------------------
//VERTEX AND PIXEL SHADER INPUTS
//--------------------------------------------------------------------------------------
struct VSInputType
{
	float3 position : POSITION0;
	float3 size : POSITION1;
};

struct GSInputType
{
	float3 position : POSITION0;
	float3 size : POSITION1;
};

struct PSInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
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
	vout.size = vin.size;
	return vout;
}
//--------------------------------------------------------------------------------------
// GEOMETRY SHADER
//--------------------------------------------------------------------------------------
float4 Transform( float3 position )
{
	float4 pos = mul( float4(position,1.0f), mat_view );
	return mul( pos, mat_proj );
}
[maxvertexcount(56)]
void GS( point GSInputType gin[1], inout TriangleStream<PSInputType> gout )
{
	PSInputType v;
	float3 position;
	float3 half_size = gin[0].size * 0.5f;
	float4 positions[12];
	float2 texes[12];	
	position = gin[0].position;
	position.xy -= half_size.xy;
	position.z += half_size.z;

	texes[0] = float2( 0.0f, 0.0f );
	positions[0] = Transform(position);

	position.z -= gin[0].size.z * ( 1.0f - BOX_CURVATURE );
	texes[1] = texes[0]; texes[1].x += gin[0].size.z * ( 1.0f - BOX_CURVATURE ) * 0.5f;
	positions[1] = Transform(position);

	position.z -= gin[0].size.z * BOX_CURVATURE;
	position.x += gin[0].size.x * BOX_CURVATURE;
	texes[2] = texes[1]; texes[2].x += gin[0].size.x * BOX_CURVATURE * 1.4f;
	positions[2] = Transform(position);

	position.x += gin[0].size.x * ( 1.0f - BOX_CURVATURE - BOX_CURVATURE );
	texes[3] = texes[2]; texes[3].x += gin[0].size.x * ( 1.0f - BOX_CURVATURE - BOX_CURVATURE );
	positions[3] = Transform(position);

	position.x += gin[0].size.x * BOX_CURVATURE;
	position.z += gin[0].size.z * BOX_CURVATURE;
	texes[4] = texes[3]; texes[4].x += gin[0].size.x * BOX_CURVATURE * 1.4f;
	positions[4] = Transform(position);

	position.z += gin[0].size.z * ( 1.0f - BOX_CURVATURE );
	texes[5] = texes[4]; texes[5].x += gin[0].size.z * ( 1.0f - BOX_CURVATURE );
	positions[5] = Transform(position);

	position = gin[0].position;
	position.yz += half_size.yz;
	position.x -= half_size.x;

	texes[6] = float2( 0.0f, gin[0].size.y );
	positions[6] = Transform(position);

	position.z -= gin[0].size.z * ( 1.0f - BOX_CURVATURE );
	texes[7] = texes[6]; texes[7].x += gin[0].size.z * ( 1.0f - BOX_CURVATURE ) * 0.5f;
	positions[7] = Transform(position);

	position.z -= gin[0].size.z * BOX_CURVATURE;
	position.x += gin[0].size.x * BOX_CURVATURE;
	texes[8] = texes[7]; texes[8].x += gin[0].size.x * BOX_CURVATURE * 1.4f;
	positions[8] = Transform(position);

	position.x += gin[0].size.x * ( 1.0f - BOX_CURVATURE - BOX_CURVATURE );
	texes[9] = texes[8]; texes[9].x += gin[0].size.x * ( 1.0f - BOX_CURVATURE - BOX_CURVATURE );
	positions[9] = Transform(position);

	position.x += gin[0].size.x * BOX_CURVATURE;
	position.z += gin[0].size.z * BOX_CURVATURE;
	texes[10] = texes[9]; texes[10].x += gin[0].size.x * BOX_CURVATURE * 1.4f;
	positions[10] = Transform(position);

	position.z += gin[0].size.z * ( 1.0f - BOX_CURVATURE );
	texes[11] = texes[10]; texes[11].x += gin[0].size.z * ( 1.0f - BOX_CURVATURE );
	positions[11] = Transform(position);

	// left surface
	v.normal = float3( -1, 0, 0 );
	v.tangent = float3( 0, 0, -1 );
	v.binormal = float3( 0, 1, 0 );
	v.normal = mul( v.normal, (float3x3)mat_view );
	v.tangent = mul( v.tangent, (float3x3)mat_view );
	v.binormal = mul( v.binormal, (float3x3)mat_view );
	v.tex = texes[0];
	v.position = positions[0];
	gout.Append(v);
	
	v.tex = texes[1];
	v.position = positions[1];
	gout.Append(v);
	
	v.tex = texes[6];
	v.position = positions[6];
	gout.Append(v);
	
	v.tex = texes[7];
	v.position = positions[7];
	gout.Append(v);
	gout.RestartStrip();

	//left front
	v.normal = float3( -1, 0, -1 );
	v.tangent = float3( 1, 0, -1 );
	v.binormal = float3( 0, 1, 0 );
	v.normal = mul( v.normal, (float3x3)mat_view );
	v.tangent = mul( v.tangent, (float3x3)mat_view );
	v.binormal = mul( v.binormal, (float3x3)mat_view );
	
	v.tex = texes[1];
	v.position = positions[1];
	gout.Append(v);
	
	v.tex = texes[2];
	v.position = positions[2];
	gout.Append(v);
	
	v.tex = texes[7];
	v.position = positions[7];
	gout.Append(v);
	
	v.tex = texes[8];
	v.position = positions[8];
	gout.Append(v);
	gout.RestartStrip();
	//front surface
	v.normal = float3( 0, 0, -1 );
	v.tangent = float3( 1, 0, 0 );
	v.binormal = float3( 0, 1, 0 );
	v.normal = mul( v.normal, (float3x3)mat_view );
	v.tangent = mul( v.tangent, (float3x3)mat_view );
	v.binormal = mul( v.binormal, (float3x3)mat_view );

	v.tex = texes[2];
	v.position = positions[2];
	gout.Append(v);
	
	v.tex = texes[3];
	v.position = positions[3];
	gout.Append(v);
	
	v.tex = texes[8];
	v.position = positions[8];
	gout.Append(v);
	
	v.tex = texes[9];
	v.position = positions[9];
	gout.Append(v);
	gout.RestartStrip();

	//front right
	v.normal = float3( 1, 0, -1 );
	v.tangent = float3( 1, 0, 1 );
	v.binormal = float3( 0, 1, 0 );
	v.normal = mul( v.normal, (float3x3)mat_view );
	v.tangent = mul( v.tangent, (float3x3)mat_view );
	v.binormal = mul( v.binormal, (float3x3)mat_view );

	v.tex = texes[3];
	v.position = positions[3];
	gout.Append(v);
	
	v.tex = texes[4];
	v.position = positions[4];
	gout.Append(v);
	
	v.tex = texes[9];
	v.position = positions[9];
	gout.Append(v);
	
	v.tex = texes[10];
	v.position = positions[10];
	gout.Append(v);
	gout.RestartStrip();

	//right face
	v.normal = float3( 1, 0, 0 );
	v.tangent = float3( 0, 0, -1 );
	v.binormal = float3( 0, 1, 0 );
	v.normal = mul( v.normal, (float3x3)mat_view );
	v.tangent = mul( v.tangent, (float3x3)mat_view );
	v.binormal = mul( v.binormal, (float3x3)mat_view );
	
	v.tex = texes[4];
	v.position = positions[4];
	gout.Append(v);
	
	v.tex = texes[5];
	v.position = positions[5];
	gout.Append(v);
	
	v.tex = texes[10];
	v.position = positions[10];
	gout.Append(v);
	
	v.tex = texes[11];
	v.position = positions[11];
	gout.Append(v);
	gout.RestartStrip();

	//top surface
	v.normal = float3( 0, 1, 0 );
	v.tangent = float3( -1, 0, 0 );
	v.binormal = float3( 0, 0, 1 );
	v.normal = mul( v.normal, (float3x3)mat_view );
	v.tangent = mul( v.tangent, (float3x3)mat_view );
	v.binormal = mul( v.binormal, (float3x3)mat_view );
	v.tex = texes[1];
	v.position = positions[6];
	gout.Append(v);
	
	v.position = positions[11];
	gout.Append(v);

	v.position = positions[7];
	gout.Append(v);

	v.position = positions[10];
	gout.Append(v);
	
	v.position = positions[8];
	gout.Append(v);
	
	v.position = positions[9];
	gout.Append(v);
	gout.RestartStrip();
	
	//bottom surface
	v.normal = float3( 0, -1, 0 );
	v.tangent = float3( 1, 0, 0 );
	v.binormal = float3( 0,0,1 );
	v.normal = mul( v.normal, (float3x3)mat_view );
	v.tangent = mul( v.tangent, (float3x3)mat_view );
	v.binormal = mul( v.binormal, (float3x3)mat_view );
	v.position = positions[0];
	gout.Append(v);
	
	v.position = positions[5];
	gout.Append(v);

	v.position = positions[1];
	gout.Append(v);

	v.position = positions[4];
	gout.Append(v);
	
	v.position = positions[2];
	gout.Append(v);
	
	v.position = positions[3];
	gout.Append(v);
	gout.RestartStrip();
}
//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
PSOutputType PS( PSInputType pin )
{ 
	PSOutputType pout;

	float3 normal = pin.normal;
	float2 bump = normal_map.Sample( sample_linear_wrap, pin.tex );
	bump -= 0.5f;
	bump *= 4.0f;
	normal += bump.x * pin.tangent;
	normal += bump.y * pin.binormal;
	pout.normal_flag.xyz = normal;
	pout.normal_flag.w = 1;
	pout.diffuse = diffuse_map.Sample( sample_linear_wrap, pin.tex );
//	pout.diffuse = float4( 0.9f, 0.9f, 0.0f, 1.0f );
	pout.view_position.w = 0;
	return pout;
}
//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 TexturedBoxShader
{
	pass pass0
	{
		SetBlendState( noblend, float4(0,0,0,0), 0xffffffff );
		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( CompileShader( gs_4_0, GS() ) );		
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }

}
