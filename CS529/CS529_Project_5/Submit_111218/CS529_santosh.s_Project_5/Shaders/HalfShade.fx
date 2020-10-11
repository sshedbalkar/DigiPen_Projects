///Base Sprite Shader
///Authors: Chris Peters

float4x4 WorldViewProj : WorldViewProjection;
float4 color = {1,1,1,1};
texture texture0;
float2 offset = {0.25, 0.25};
//
sampler Sampler0
{
    Texture = texture0;
    MipFilter = Linear;
    MinFilter = Linear;
    MagFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};


struct VS_INPUT
{
    float3 position	: POSITION;
	float2 tex0 : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 screenposition : POSITION;
	float2 tex0  : TEXCOORD0;
};

VS_OUTPUT VertexShader0( VS_INPUT IN )
{
	VS_OUTPUT OUT;
	OUT.screenposition = mul( float4(IN.position, 1) , WorldViewProj);
	OUT.tex0 = IN.tex0;
	return OUT;
}

float4 PixelShader0( VS_OUTPUT IN ) : COLOR
{
	//return tex2D( Sampler0 , IN.tex0 ) * color;
	return tex2D(Sampler0 , IN.tex0)*IN.tex0.x*5;
	float4 Color;
	Color = tex2D(Sampler0, IN.tex0.xy)*IN.tex0.x*5;
	Color -= tex2D(Sampler0, IN.tex0.xy+0.0001)*10.0f;
	Color += tex2D(Sampler0, IN.tex0.xy-0.0001)*10.0f;
	//
	return Color;
	}

technique Technique0
{
	pass Pass0
	{
		Lighting = FALSE;
		AlphaBlendEnable = TRUE;
		VertexShader = compile vs_2_0 VertexShader0();
		PixelShader  = compile ps_2_0 PixelShader0();
	}
}



