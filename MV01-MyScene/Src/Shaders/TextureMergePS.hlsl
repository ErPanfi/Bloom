//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------


//texture scena 
Texture2D baseTexture : register(t0);
Texture2D otherTexture : register(t1);
SamplerState	texSampler : register(s0);

struct PixelShaderInput
{
	float4 posCoord : POSITION;
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return float4(baseTexture.Sample(texSampler, input.texCoord).rgb + 0.25*(otherTexture.Sample(texSampler, input.texCoord).rgb), 1.0);
}
