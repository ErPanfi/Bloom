Texture2D		inputTexture : register(t0);
SamplerState	texSampler : register(s0);

cbuffer BlurParameters : register(cb0)
{
	int level;
}

struct PixelShaderInput
{
	float4 posCoord : POSITION;
    float2 texCoord : TEXCOORD0;
};

static const float blurHorizSize = 0.005f;
static const float blurHorizWeights[] = {0.16f, 0.15f, 0.12f, 0.09f, 0.05f};

float4 main(PixelShaderInput input) : SV_TARGET
{

	//float screenWidth = 640.0f;
	//float texelSize = 1.0f / screenWidth;

	//float weight0 = 1.0f;
 //   float weight1 = 0.9f;
 //   float weight2 = 0.55f;
 //   float weight3 = 0.18f;
 //   float weight4 = 0.1f;

	//float normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	//// Normalize the weights.
 //   weight0 = weight0 / normalization;
 //   weight1 = weight1 / normalization;
 //   weight2 = weight2 / normalization;
 //   weight3 = weight3 / normalization;
 //   weight4 = weight4 / normalization;



	//float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//
	//	

	//color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize * -4.0f, 0.0f)) * weight4;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize * -3.0f, 0.0f)) * weight3;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize * -2.0f, 0.0f)) * weight2;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize * -1.0f, 0.0f)) * weight1;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize *  0.0f, 0.0f)) * weight0;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize *  1.0f, 0.0f)) * weight1;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize *  2.0f, 0.0f)) * weight2;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize *  3.0f, 0.0f)) * weight3;
 //   color += inputTexture.Sample(texSampler, input.texCoord + float2(texelSize *  4.0f, 0.0f)) * weight4;

	//

	//color.a = 1.0f;

	//return color;



	float screenWidth = 640.0f;
	float texelSize = 1.0f / screenWidth;


	float3 rgb = float3(0.0, 0.0, 0.0);
	float blurMaxWeight = 0.16f; 

	[unroll(15)] for(int i = 0; i < level; i++)
	{
		rgb += inputTexture.Sample(texSampler, input.texCoord + float2(-i * texelSize, 0.0f)).rgb * (blurMaxWeight - i * 0.01f);
		rgb += inputTexture.Sample(texSampler, input.texCoord + float2(+i * texelSize, 0.0f)).rgb * (blurMaxWeight - i * 0.01f);
	}

	return float4(rgb, 1.0f);
}
