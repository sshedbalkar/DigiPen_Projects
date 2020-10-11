#include "states.fxh"

cbuffer PerFrameConstants
{
	matrix mat_view;
	matrix mat_proj;
};

//--------------------------------------------------------------------------------------
//VERTEX AND PIXEL SHADER INPUTS
//--------------------------------------------------------------------------------------
struct VSInputType
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
	int bone : BONE;
	row_major float4x4 transform : TRANSFORM;
};

struct PSInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct PSOutputType
{
	float4 color : SV_Target0;
	float4 extra : SV_Target1;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
PSInputType VS( VSInputType vin )
{
	PSInputType vout;
	float4 pos = float4( vin.position, 1.0f );
	vout.position = mul( pos, vin.transform );
	vout.position = mul( vout.position, mat_view );
	vout.position = mul( vout.position, mat_proj );
	vout.normal = mul( vin.normal, (float3x3)vin.transform );
	vout.color = vin.color;
	return vout;
}
//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
PSOutputType PS( PSInputType pin )
{ 
	PSOutputType pout;

	float3 light_direction = float3( 0.48f, -0.8f, 0.36f );
	float intensity = 0.2f;
	intensity += saturate(dot( light_direction, pin.normal ));

//	pout.color = pin.color * float4( intensity.xxx, 1.0f );
	pout.extra.w = pin.position.z / pin.position.w;
	pout.color = float4( 1.0,0.0,0.0,0.5 );
	return pout;
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 StaticMeshShader
{
    pass pass0
    {
		SetBlendState( noblend, float4(0,0,0,0), 0xffffffff );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}
