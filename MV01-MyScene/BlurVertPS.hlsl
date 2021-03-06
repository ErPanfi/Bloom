static const int MAX_LEVEL = 8;

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
static const float blurVertWeights[] = {0.16f, 0.15f, 0.12f, 0.09f, 0.06f};

float4 main(PixelShaderInput input) : SV_TARGET
{

	float screenHeight = 768.0f;
	float texelSize = 4.0f / screenHeight;


	float3 rgb = float3(0.0, 0.0, 0.0);

	[unroll(5)] for(int i = 0; i < 5 && i < level; i++)
	{
		//vertical blur
		rgb += inputTexture.Sample(texSampler, input.texCoord + float2(0.0f, (-i * texelSize))).rgb * blurVertWeights[i];
		rgb += inputTexture.Sample(texSampler, input.texCoord + float2(0.0f, (+i * texelSize))).rgb * blurVertWeights[i];
	}

	return float4(rgb, 1.0f);
}
