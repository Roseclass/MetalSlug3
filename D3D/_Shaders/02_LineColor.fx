#include "00_Global.fx"

struct VertexColor
{
    float4 Position : Position;
    float4 Color : Color;
};

struct VertexColorOutput
{
    float4 Position : SV_Position;
    float4 Color : Color;
};


VertexColorOutput VS(VertexColor input)
{
	VertexColorOutput output;
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);	
	output.Color = input.Color;

    return output;
}

float4 PS(VertexColorOutput input) : SV_Target
{
    return input.Color;
}

technique11 T0
{
	P_RS_BS_VP(P0, CullMode_None, AlphaBlend, VS, PS)
}