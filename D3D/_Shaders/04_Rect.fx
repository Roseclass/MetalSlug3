#include "00_Global.fx"
float lerpTime;
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Uv = input.Uv;
	
    return output;
}
float3 LinearToGamma(float3 color)
{
	return pow(color, 1 / 2.2);
}
float4 PS(VertexOutput input) : SV_Target
{	
	float4 sRGBA = DiffuseMap.Sample(PointSampler, input.Uv);
	float3 sRGB = float3(sRGBA.x, sRGBA.y, sRGBA.z);
	float3 RGB = LinearToGamma(sRGB);
	return float4(RGB, sRGBA.w);
}
float4 PS_Wrap(VertexOutput input) : SV_Target
{
	float4 sRGBA = DiffuseMap.Sample(PointSampler_Wrap, input.Uv);
	float3 sRGB = float3(sRGBA.x, sRGBA.y, sRGBA.z);
	float3 RGB = LinearToGamma(sRGB);
	return float4(RGB, sRGBA.w);
}
float4 PS_Test(VertexOutput input) : SV_Target
{
	float4 color = DiffuseMap.Sample(PointSampler,input.Uv);
	color = mul(color, 1.2f);
	[flatten]
	if (color.a >= 1.0f) color.a = 1.0f;
	return color;
}
float4 PS_Lerp(VertexOutput input) : SV_Target
{
	float4 sRGBA = DiffuseMap.Sample(PointSampler, input.Uv);
	float3 sRGB = float3(sRGBA.x, sRGBA.y, sRGBA.z);
	float3 RGB = LinearToGamma(sRGB);
	float3 temp = float3(0.0f, 0.0f, 0.0f);
	RGB = lerp(temp, RGB, lerpTime);
	return float4(RGB, sRGBA.w);
}
float4 PS_Lerp_Hit(VertexOutput input) : SV_Target
{
	float4 sRGBA = DiffuseMap.Sample(PointSampler, input.Uv);
	float3 sRGB = float3(sRGBA.x, sRGBA.y, sRGBA.z);
	float3 RGB = LinearToGamma(sRGB);
	float3 temp = float3(1.0f, 127.0f/255.0f, 0.0f);
	RGB = lerp(RGB, temp, lerpTime*3.0f);
	return float4(RGB, sRGBA.w);
}
technique11 T0
{
	P_RS_BS_VP(P0, CullMode_None, AlphaBlend_AlphaToCoverage, VS, PS)
	P_RS_BS_VP(P1, CullMode_None, AlphaBlend_AlphaToCoverage, VS, PS_Lerp)
	P_RS_BS_VP(P2, CullMode_None, AlphaBlend_AlphaToCoverage, VS, PS_Lerp_Hit)
	P_RS_BS_VP(P3, CullMode_None, AlphaBlend_AlphaToCoverage, VS, PS_Wrap)
	P_RS_BS_VP(P4, CullMode_None, AlphaBlend_AlphaToCoverage, VS, PS_Test)
}