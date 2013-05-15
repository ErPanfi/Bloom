//--------------------------------------------------------------------------------------
// Copyright (c) Luca Giona 2013. Do not distribute. (Master Game Development - University of Verona)
//--------------------------------------------------------------------------------------


//texture scena 
Texture2D		textures[2];

SamplerState	texSampler;

struct PixelShaderInput
{
	float4 posCoord : POSITION;
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 baseTexel = textures[0].Sample(texSampler, input.texCoord);
	float4 otherTexel = textures[1].Sample(texSampler, input.texCoord);
	
	if(otherTexel.a != 0 && otherTexel.r && otherTexel.b && otherTexel.g)
	{
		float baseFactor = 0.60;
		return baseTexel + otherTexel * (1 - baseFactor);
	}
	else
		return baseTexel;
}
