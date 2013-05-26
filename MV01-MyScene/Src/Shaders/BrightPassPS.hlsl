Texture2D		inputTexture : register(t0);
SamplerState	texSampler : register(s0);

cbuffer BrightThreshold : register(cb0)
{
	float threshold;
}

struct PixelShaderInput
{
	float4 posCoord : POSITION;
    float2 texCoord : TEXCOORD0;
};


//will do 4x4 downscaling and brightpass
float4 main(PixelShaderInput input) : SV_TARGET
{
	//first sample the 4x4 pixel square
	float3 baseColor = float3(0.0f, 0.0f, 0.0f);
	float2 sampleTexel = 4 * input.texCoord;
	[unroll(16)]for(int i = 0; i < 16; i++)
	{
		int xOffset = i % 4;
		int yOffset = i / 4;
		baseColor += inputTexture.Sample(texSampler, sampleTexel + float2(xOffset, yOffset)).rgb;
	}

	//then average the obtained value
	baseColor /= 16.0f;

	//output value if it pass the bright test
	if(threshold <= max(baseColor.r, max(baseColor.g, baseColor.b)))
		return float4(baseColor, 1.0);
	else
		return float4(0.0, 0.0, 0.0, 1.0);
}