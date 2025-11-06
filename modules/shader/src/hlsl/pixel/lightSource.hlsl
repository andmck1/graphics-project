Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};

float4 getTextureColour(Texture2D _objectTexture, SamplerState _textureSampler, float2 uv)
{
	return _objectTexture.Sample(_textureSampler, uv);
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	textureColour = getTextureColour(objectTexture, textureSampler, input.tex);
    return textureColour;
}