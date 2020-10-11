float4x4 WorldViewProj: WorldViewProjection;
float4 color = {1, 1, 1, 1};
texture texture0;

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
	float3 position: POSITION;
	float2 tex0: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 screenposition: POSITION;
	float2 tex0: TEXCOORD0;
};

VS_OUTPUT VertexShader0(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	OUT.screenposition = mul(float4(IN.position, 1), WorldViewProj);
	OUT.tex0 = IN.tex0;
	return OUT;
	//return IN.tex0;
}
float timeraslow=0.7;
float4 MyShader(VS_OUTPUT IN): COLOR0
{
	float4 Color;

	//Color = tex2D(Sampler0, IN.tex0);
	//Color.r = Color.r*sin(IN.tex0.y*100)*2;
	//Color.g = Color.g*cos(IN.tex0.y*200)*2;
	//Color.b = Color.b*sin(IN.tex0.y*300)*2;
	//
	//IN.tex0.y = IN.tex0.y * 0.5;
	//Color = tex2D( Sampler0, IN.tex0.xy);
	//
	//IN.tex0.y = IN.tex0.y + (sin(IN.tex0.x*200)*0.01);
	//Color = tex2D( Sampler0, IN.tex0.xy);
	//
	//IN.tex0.y = IN.tex0.y + (sin(timeraslow)*0.01);
	//IN.tex0.x = IN.tex0.x + (cos(timeraslow)*0.01);
	//Color = tex2D( Sampler0, IN.tex0.xy);
	//
	//Color = tex2D( Sampler0, IN.tex0.xy);
	//Color += tex2D( Sampler0, IN.tex0.xy+0.001);
	//Color += tex2D( Sampler0, IN.tex0.xy+0.002);
	//Color += tex2D( Sampler0, IN.tex0.xy+0.003);
	//Color = Color/4;
	//
	//Color.a = 1.0f;
	//Color.rgb = 0.5f;
	//Color -= tex2D( Sampler0, IN.tex0.xy-0.001)*2.0f;
	//Color += tex2D( Sampler0, IN.tex0.xy+0.001)*2.0f;
	//Color.rgb = (Color.r+Color.g+Color.b)/3.0f;
	//
	//Color.a = 1.0f;
	//Color = tex2D( Sampler0, IN.tex0.xy);
	//Color.rgb = (Color.r+Color.g+Color.b)/3.0f;
	//if (Color.r<0.2 || Color.r>0.8) Color.r = 0.0f; else Color.r = 1.0f;
	//if (Color.g<0.2 || Color.g>0.8) Color.g = 0.0f; else Color.g = 1.0f;
	//if (Color.b<0.2 || Color.b>0.8) Color.b = 0.0f; else Color.b = 1.0f;
	//
	Color = 1-tex2D( Sampler0, IN.tex0.xy);
	Color.a = 1.0f;
	return Color;
}


technique Technique0
{
	pass Pass0
	{
		Lighting = FALSE;
		AlphaBlendEnable = TRUE;
		//
		VertexShader = compile vs_2_0 VertexShader0();
		PixelShader = compile ps_2_0 MyShader();
	}
}