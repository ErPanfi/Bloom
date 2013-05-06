//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------

Texture2D		inputTexture;
SamplerState	texSampler;

cbuffer BrightThreshold : register(cb2)
{
	float threshold;
}

struct PixelShaderInput
{
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 baseColor = inputTexture.Sample(texSampler, input.texCoord).rgb;

	if(threshold <= max(baseColor.r, max(baseColor.g, baseColor.b)))
		return (baseColor, 1.0);
	else
		return (0.0, 0.0, 0.0, 1.0);
}
