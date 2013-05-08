struct VertexShaderInput
{
	float3 posCoord : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct PixelShaderInput
{
	float4 posCoord : SV_POSITION;
    float2 texCoord : TEXCOORD0;
};

PixelShaderInput main( VertexShaderInput input)
{
	PixelShaderInput output;
	output.posCoord = float4(input.posCoord, 1.0);
	output.texCoord = input.texCoord;
	return output;
}