#include "states.fxh"

cbuffer PerFrameConstants
{
	matrix mat_view;
	matrix mat_proj;
};

cbuffer PointLights
{
	float3 light_position;
};

float2 viewport;			// for calculating pixel width
Texture2D diffuse;			// {xyz_}-diffuse color
Texture2D normal_flag;		// {xyz_}-normal;	{___w} - flag(uint)
Texture2D view_position;	// {xyz_}-view position; {___w} - shininess

//--------------------------------------------------------------------------------------
//VERTEX AND PIXEL SHADER INPUTS
//--------------------------------------------------------------------------------------
struct GSInputType
{
	float2 size : SIZE;
};

struct PSInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// VERTEX SHADER
//--------------------------------------------------------------------------------------
GSInputType VS( void )
{
	GSInputType vout;
	vout.size = 1.0f / viewport;
	return vout;
}
//--------------------------------------------------------------------------------------
// GEOMETRY SHADER
//--------------------------------------------------------------------------------------
float4 Transform( float3 position )
{
	float4 pos = float4( position, 1.0f );
	pos = mul( pos, mat_view );
	pos = mul( pos, mat_proj );
	return pos;
}
[maxvertexcount(4)]
void GS( point GSInputType gin[1], inout TriangleStream<PSInputType> gout )
{
	PSInputType v;

	v.position = Transform( float3(0.0f,0.0f,0.0f) );
	v.tex = float2( 0.0f,1.0f ) ;
	gout.Append(v);
	
	v.position = Transform( float3( gin[0].size.x, 0.0f, 0.0f ) );
	v.tex = float2( 1.0f,1.0f ) ;
	gout.Append(v);
	
	v.position = Transform( float3( 0.0f, gin[0].size.y, 0.0f ) );
	v.tex = float2( 0.0f, 0.0f );
	gout.Append(v);
	
	v.position = Transform( float3( gin[0].size, 0.0f ) );
	v.tex = float2( 1.0f, 0.0f ) ;
	gout.Append(v);
}
//--------------------------------------------------------------------------------------
// PIXEL SHADER
//--------------------------------------------------------------------------------------
float4 PS( PSInputType pin ) : SV_Target
{
	float4 result = float4(0,0,0,0);	//return value(RGBI)
	float2 tex = pin.tex;				//texture coordinates
 	float4 colorM = diffuse.SampleLevel( sample_point_clamp, tex, 0.0 );	//diffuse color
	float4 normalflag = normal_flag.SampleLevel( sample_point_clamp,tex, 0.0 );
	float3 normalM = normalize(normalflag.xyz);
	uint flag = (uint)normalflag.a;

// 1. deferred shading
	if ( (flag & EFFECT_DIFFUSE) == EFFECT_DIFFUSE )
	{
		/** calculate diffuse lighting **/
		// a sample directional light source is used here, could be changed due to game design
		float intensity = 0.2f;
		intensity += saturate(dot( float3(-0.36,0.8,-0.64), normalM ));
		result.xyz += float3( intensity.xxx * colorM.xyz );
	}
	else result = colorM;
// 2. specular
	if ( (flag & EFFECT_SPECULAR ) == EFFECT_SPECULAR )
	{
		float4 positionM = view_position.SampleLevel( sample_point_clamp,tex, 0.0 );
		int shininess = (int)positionM.w;
		float3 eye_vec = -positionM.xyz;
		float3 reflect_vec = reflect( -(float3( 0,0,40) + eye_vec), normalM );
		eye_vec = normalize(eye_vec);
		reflect_vec = normalize( reflect_vec );
		result.xyz += float3( 1.0,1.0,1.0 ) * pow( saturate( dot( reflect_vec, eye_vec ) ), 30 );
	}
	return result;
}
//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 DeferredShader
{
    pass pass0
    {
		SetBlendState( noblend, float4(0,0,0,0),0xffffffff );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );		
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }    
}