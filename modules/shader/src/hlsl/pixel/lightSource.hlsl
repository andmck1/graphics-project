#include "sharedbuffertypes.h"

Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);

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
