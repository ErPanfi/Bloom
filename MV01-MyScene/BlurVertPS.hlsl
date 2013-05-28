Texture2D		inputTexture : register(t0);
SamplerState	texSampler : register(s0);

cbuffer BlurParameters : register(cb0)
{
	int		level;
	float	blurVertSize;
}

struct PixelShaderInput
{
	float4 posCoord : POSITION;
    float2 texCoord : TEXCOORD0;
};

//static const float blurVertSize = 0.01f;
static const float blurVertWeights[] = {0.16f, 0.15f, 0.12f, 0.09f, 0.05f};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float	blurWeight = 0.16f;
	float3	rgb = inputTexture.Sample(texSampler, float2(input.texCoord.x, input.texCoord.y)).rgb * blurWeight;
	float	totalWeight = blurWeight;

	for(int i = 0; i < level && blurWeight > 0.0f; i++, blurWeight -= 0.01f)
	{
		//vertical blur
		rgb += inputTexture.Sample(texSampler, float2(input.texCoord.x, input.texCoord.y - (i * blurVertSize))).rgb * blurWeight;
		rgb += inputTexture.Sample(texSampler, float2(input.texCoord.x, input.texCoord.y + (i * blurVertSize))).rgb * blurWeight;
		totalWeight += 2 * blurWeight;
	}

	//rgb /= totalWeight;

	return float4(rgb, 1.0f);
}
