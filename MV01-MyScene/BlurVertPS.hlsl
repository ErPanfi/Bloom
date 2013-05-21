static const int MAX_LEVEL = 5;

Texture2D		inputTexture : register(t0);
SamplerState	texSampler : register(s0);

cbuffer BlurParameters : register(cb0)
{
	int		level;
	float	blurSize;
	float	blurWeights[MAX_LEVEL];
}

struct PixelShaderInput
{
	float4 posCoord : POSITION;
    float2 texCoord : TEXCOORD0;
};

//static const float blurVertSize = 0.0025f;
//static const float blurVertWeights[] = {0.16f, 0.15f, 0.12f, 0.09f, 0.05f};

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 rgb = float3(0.0, 0.0, 0.0);
	//float blurMaxWeight = 0.16f;

	[unroll(MAX_LEVEL)] for(int i = 0; i < level; i++)
	{
		//vertical blur
		rgb += inputTexture.Sample(texSampler, float2(input.texCoord.x, input.texCoord.y - (i * blurSize))).rgb * blurWeights[i]; //(blurMaxWeight - i * 0.01f);
		rgb += inputTexture.Sample(texSampler, float2(input.texCoord.x, input.texCoord.y + (i * blurSize))).rgb * blurWeights[i]; //(blurMaxWeight - i * 0.01f);
	}

	return float4(rgb, 1.0f);
}
