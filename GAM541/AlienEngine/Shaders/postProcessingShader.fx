#include "states.fxh"
#define MOTION_BLUR_FACTOR		0.1f		// factor * frametime = t; color = prev*(1-t) + cur*t

cbuffer PerFrameConstants
{
	matrix mat_view;
	matrix mat_proj;
};

cbuffer PointLights
{
	float3 light_position;
};

Texture2D color;			// {xyz_}-diffuse color
Texture2D normal_flag;		// {xyz_}-normal;	{___w} - flag(uint)
Texture2D view_position;	// {xyz_}-view position; {___w} - shininess(useless here)
Texture2D previous_frame;	// for motion blur

float2 viewport;			// for calculating pixel width
float frametime;			// for accurate motion blur


//--------------------------------------------------------------------------------------
//	filter functions declaration
//--------------------------------------------------------------------------------------
float3 oilpainting( float2 tex );
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
inline float luma( float4 color )
{
	return dot(color.rgb, float3(0.299, 0.587, 0.114) );
}
float4 PS( PSInputType pin ) : SV_Target
{
	float2 tex = pin.tex;	//texture coordinates
	float4 result = color.SampleLevel( sample_point_clamp, tex, 0.0 );	//return value(RGBL)
 	//float4 colorM = color.SampleLevel( sample_point, tex, 0.0 );
	float4 normalflag = normal_flag.SampleLevel( sample_point_clamp,tex, 0.0 );
	float3 normalM = normalize(normalflag.xyz);
	uint flag = (uint)normalflag.a;

// 1. outline
	if ( (flag & EFFECT_OUTLINE ) == EFFECT_OUTLINE )
	{
		float depthM = view_position.SampleLevel( sample_point_clamp, pin.tex + float2( 0.0, 0.0 ) * viewport, 0.0 ).z;

		float thickness = 2.0f;
		float depthN = view_position.SampleLevel( sample_point_clamp, pin.tex + float2( 0.0,-thickness ) * viewport, 0.0 ).z;
		float depthW = view_position.SampleLevel( sample_point_clamp, pin.tex + float2(-thickness, 0.0 ) * viewport, 0.0 ).z;
		float depthS = view_position.SampleLevel( sample_point_clamp, pin.tex + float2( 0.0, thickness ) * viewport, 0.0 ).z;
		float depthE = view_position.SampleLevel( sample_point_clamp, pin.tex + float2( thickness, 0.0 ) * viewport, 0.0 ).z;
		
//		float min_depth = min( depthN, min( depthW, min( depthS, depthE ) ) );
		float max_depth = max( depthN, max( depthW, max( depthS, depthE ) ) );
	
//		float range_depth = max_depth - min_depth;
		float contrast_depth = max_depth / depthM;

		if ( contrast_depth > 1.02f ) result.xyz = float3( 0,0,0 );
	}
	//if ( flag == 2 )
		//result.xyz = oilpainting(tex);

// last but one:
//  motion blur (blend with previous frame)
	float t = saturate(MOTION_BLUR_FACTOR * frametime );
	float4 prev = previous_frame.SampleLevel( sample_point_clamp, pin.tex, 0.0 );
	result *= t;
	result += prev * ( 1.0f - t );

// final step : 
//  calculate and return RGBL value(for fxaa)
	result.a = luma(result);
	return result;
}
//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 PostProcessingShader
{
    pass pass0
    {
		SetBlendState( noblend, float4(0,0,0,0),0xffffffff );
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( CompileShader( gs_4_0, GS() ) );		
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }    
}
//--------------------------------------------------------------------------------------
// filter functions
//--------------------------------------------------------------------------------------
#define LevelOfIntensity		5
#define BrushWidth				3
float3 oilpainting( float2 tex )
{
	float2 offset = float2(0.0f,0.0f);
	float3 average[LevelOfIntensity];
	int pixel_count[LevelOfIntensity];
	for ( int i = 0; i < LevelOfIntensity; ++i ) { average[i]=float3(0,0,0); pixel_count[i]=0; }
	int max_count = 0;
	int max_index = 0;
	for ( int i = -BrushWidth; i <= BrushWidth; ++i )
	{
		int rj = BrushWidth - abs(i);
		for ( int j = -rj; j <= rj; ++j )
		{
			offset = float2( (float)i, (float)j ) * viewport;
			float3 rgb = color.SampleLevel( sample_point_clamp, tex + offset, 0.0 ).xyz;
			int intensity = (int)((float)LevelOfIntensity * dot(rgb, 0.3333f ));
			--intensity;
			++pixel_count[intensity];
			average[intensity] += rgb;
			if ( pixel_count[intensity]> max_count ) { max_index = intensity; max_count = pixel_count[intensity]; }
		}
	}
	return average[max_index] / (float)max_count;
}
