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
	float2 texel00Coords = floor(input.texCoord / 4);
	float4 otherTexel00 = textures[1].Sample(texSampler, texel00Coords);
	float2 texel01Coords = float2(ceil(input.texCoord.x / 4), floor(input.texCoord.y / 4));
	float4 otherTexel01 = textures[1].Sample(texSampler, texel01Coords);
	float2 texel10Coords = float2(floor(input.texCoord.x / 4), ceil(input.texCoord.y / 4));
	float4 otherTexel10 = textures[1].Sample(texSampler, texel10Coords);
	float2 texel11Coords = ceil(input.texCoord / 4);
	float4 otherTexel11 = textures[1].Sample(texSampler, texel11Coords);

	float xRightWeight = (input.texCoord.x / 4) - otherTexel00.x;
	float yDownWeight = (input.texCoord.y / 4) - otherTexel00.y;

	float4 otherTexel = (1 - yDownWeight) * ((1 - xRightWeight) * otherTexel00 + xRightWeight * otherTexel01) + 
						yDownWeight * ((1 - xRightWeight) * otherTexel10 + xRightWeight * otherTexel11);
	
	if(otherTexel.a != 0 && otherTexel.r && otherTexel.b && otherTexel.g)
	{
		float baseFactor = 0.60;
		return baseTexel + otherTexel * (1 - baseFactor);
	}
	else
		return baseTexel;
}
