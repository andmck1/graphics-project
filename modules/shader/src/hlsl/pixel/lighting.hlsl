#include "buffer.h"

Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);

cbuffer AmbientLightBuffer: register(b0)
{
	AmbientLightBuffer ambientLightData;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 calculateAmbientLighting(float4 colour)
{
	return colour;
}

float4 calculateLighting(float4 ambientColour)
{
	float4 calculatedLightColour;
	float4 ambientLighting;
	// float intensity = saturate(dot(normal, lightDirection));
	// float4 colour = saturate(diffuse * intensity);
	ambientLighting = calculateAmbientLighting(ambientColour);

	calculatedLightColour = ambientLighting;
	return calculatedLightColour;
}

float4 getTextureColour(Texture2D _objectTexture, SamplerState _textureSampler, float2 uv)
{
	return _objectTexture.Sample(_textureSampler, uv);
}

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour;
	textureColour = getTextureColour(objectTexture, textureSampler, input.tex);
	// lightColour = calculateLighting(-lightData.direction.xyz, input.normal, lightData.colour);
	lightColour = calculateLighting(ambientLightData.colour);
	
    return textureColour * lightColour;
}
