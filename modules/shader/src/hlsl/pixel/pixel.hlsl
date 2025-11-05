#include "buffer.h"

Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);

cbuffer LightBuffer : register(b0)
{
	LightBuffer lightData;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 lightColour;
	float4 textureColour = objectTexture.Sample(textureSampler, input.tex);
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	lightColour = calculateLighting(-lightData.direction.xyz, input.normal, lightData.colour);
	
	
    return textureColour * lightColour;
}
