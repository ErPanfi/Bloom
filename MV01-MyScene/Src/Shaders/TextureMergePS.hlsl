//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------


//texture scena 
Texture2D baseTexture;
Texture2D otherTexture;
SamplerState	texSampler;

struct PixelShaderInput
{
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return (baseTexture.Sample(texSampler, input.texCoord).rgb + 0.25*(otherTexture.Sample(texSampler, input.texCoord).rgb), 1.0);
}
