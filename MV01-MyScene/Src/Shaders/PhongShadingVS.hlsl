//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

cbuffer TransformsBuffer : register(cb0)
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float3 CameraPosition;
};

struct VertexShaderInput
{
	float3 pos	: POSITION;
	float3 norm : NORMAL;
};

struct PixelShaderInput
{
    float4 pos				: SV_POSITION;
	float3 wNormal			: NORMAL;
	float3 viewDirection	: VIEWDIRECTION;
	float3 wPos				: WORLDPOSITION;
};

PixelShaderInput PhongShadingVS(VertexShaderInput input)
{
	PixelShaderInput output;

	float4 worldSpacePos = mul(World, float4(input.pos, 1.0f));
	float4 cameraSpacePos = mul(View, worldSpacePos);

	output.pos = mul(Projection, cameraSpacePos);
	output.wNormal = mul((float3x3)World, input.norm);
	output.viewDirection = normalize(CameraPosition - worldSpacePos.xyz);
	output.wPos = worldSpacePos.xyz;

	return output;
}
