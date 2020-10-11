SamplerState sample_point_clamp
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};

SamplerState sample_point_wrap
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState sample_linear_wrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

BlendState add_diffuse
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};
BlendState noblend
{
};
DepthStencilState depthdisable
{
	DepthEnable				= FALSE;
};
DepthStencilState depthenable
{
	DepthEnable				= TRUE;
};

static const float2 screen = { 1.0f/1280.0f, 1.0f/720.0f };

#define EFFECT_DIFFUSE					1
#define EFFECT_CELSHADING				2
#define EFFECT_SPECULAR					4
#define EFFECT_OUTLINE					8
#define EFFECT_PAINTING

#define PT_LAUNCHER						1
#define PT_GRAVITYAFFECTING				2
#define PT_SHRINKING					4
#define PT_ROTATING						8
#define PT_RANDOMROTATION				16