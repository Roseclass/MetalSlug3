Texture2D DiffuseMap;
cbuffer CB_PerFrame
{
	matrix View;
	matrix Projection;
	matrix VP;
	float time;
}
cbuffer CB_World
{
	matrix World;
}
struct VertexInput
{
    float4 Position : Position;
	float2 Uv : Uv;
};

struct VertexOutput
{
    float4 Position : SV_Position;
	float2 Uv : Uv;
};
//World
float4 WorldPosition(float4 position)
{
	return mul(position, World);
}
//ViewProjection
float4 ViewProjection(float4 position)
{
	position = mul(position, View);
	return mul(position, Projection);
}

SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;

	AddressU = Mirror;
	AddressV = Mirror;
};

SamplerState PointSampler_Wrap
{
	Filter = MIN_MAG_MIP_POINT;

	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

RasterizerState CullMode_None
{
	CullMode = None;
	DepthClipEnable = false;
};

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

BlendState AlphaBlend
{
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;      
	SrcBlend[0] = SRC_ALPHA;         
	BlendOp[0] = ADD;

	SrcBlendAlpha[0] = ONE;
	DestBlendAlpha[0] = ONE;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState AlphaBlend_AlphaToCoverage
{
	AlphaToCoverageEnable = true;

	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = One;
	RenderTargetWriteMask[0] = 0x0F;
};


#define P_VP(name, vs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}
#define P_RS_VP(name, rs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}
#define P_BS_VP(name, bs, vs, ps) \
pass name \
{ \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}
#define P_RS_BS_VP(name, rs, bs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}