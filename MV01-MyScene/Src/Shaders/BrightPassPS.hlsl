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

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 baseColor = inputTexture.Sample(texSampler, input.texCoord).rgb;

	if(threshold <= max(baseColor.r, max(baseColor.g, baseColor.b)))
		return float4(baseColor, 1.0);
	else
		return float4(0.0, 0.0, 0.0, 1.0);
}
