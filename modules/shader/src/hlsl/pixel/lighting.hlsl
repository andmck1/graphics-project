#include "lightingBuffer.h"

Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);

cbuffer AmbientLightBuffer: register(b0)
{
	AmbientLightBufferType ambientLightData;
}

StructuredBuffer<PointLightBufferType> PointLightBuffer : register(t1);
StructuredBuffer<DirectionalLightBufferType> DirectionalLightBuffer : register(t2);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};

float3 calculateSpecularLighting(
	float3 specularColour, 
	float specularPower,
	float3 viewWorldPosition, 
	float3 lightVector, 
	float3 fragmentNormal
)
{
    float3 halfVector = normalize(viewWorldPosition + lightVector);
    float specularIntensity = pow(max(dot(fragmentNormal, halfVector), 0.0), specularPower);
    return specularIntensity * specularColour;

}

float3 calculateDiffuseLighting(float3 diffuseColour, float3 normal, float3 lightVector)
{
	float3 diffuseLighting;
    float diffuseIntensity = max(dot(normal, lightVector), 0.f);
    diffuseLighting = diffuseColour * diffuseIntensity;
	return diffuseLighting;
}

float3 calculateDirectionalLighting(
	DirectionalLightBufferType directionalLightData,
	float3 fragmentWorldPosition, 
	float3 fragmentNormal, 
	float3 viewWorldPosition
)
{
	float3 directionalLighting;
	float3 diffuse = {0.f, 0.f, 0.f};
	float3 specular = {0.f, 0.f, 0.f};
	float distance;
	float attenuation = 1.f;

	if ((bool) directionalLightData.isDiffuseEnabled.r)
	{
		diffuse = calculateDiffuseLighting(
			directionalLightData.diffuseColour.rgb, 
			fragmentNormal, 
			directionalLightData.directionVector.xyz
		);
	}
	if ((bool) directionalLightData.isSpecularEnabled)
	{
		specular = calculateSpecularLighting(
			directionalLightData.specularColour.rgb,
			directionalLightData.specularPower.r,
			viewWorldPosition,
			directionalLightData.directionVector.xyz,
			fragmentNormal	
		);
	}

	directionalLighting = diffuse + specular;
	directionalLighting *= attenuation;
	return directionalLighting;
}

float3 calculatePointLighting(
	PointLightBufferType pointLightData,
	float3 fragmentWorldPosition, 
	float3 fragmentNormal, 
	float3 lightVector,
	float3 viewWorldPosition
)
{
	float3 pointLighting;
	float3 diffuse = {0.f, 0.f, 0.f};
	float3 specular = {0.f, 0.f, 0.f};
	float distance;
	float attenuation = 1.f;

	if ((bool) pointLightData.isDiffuseEnabled.r)
	{
		diffuse = calculateDiffuseLighting(
			pointLightData.diffuseColour.rgb, 
			fragmentNormal, 
			lightVector
		);
	}
	if ((bool) pointLightData.isSpecularEnabled.r)
	{
		specular = calculateSpecularLighting(
			pointLightData.specularColour.rgb,
			pointLightData.specularPower.r,
			viewWorldPosition,
			lightVector,
			fragmentNormal	
		);
	}

	if ((bool) pointLightData.isAttenuationEnabled.r)
	{
		distance = length(pointLightData.worldPosition.xyz - fragmentWorldPosition);
		attenuation = 1.0f / (
			pointLightData.attenuationCoefficients[0] + 
			(pointLightData.attenuationCoefficients[1] * distance) + 
			(pointLightData.attenuationCoefficients[2] * distance * distance)
		);
	}

	pointLighting = diffuse + specular;
	pointLighting *= attenuation;
	return pointLighting;
}

float3 calculateAmbientLighting(float3 colour)
{
	return max(colour, float3(0.f, 0.f, 0.f));
}

float3 calculateLighting(
	float3 fragmentWorldPosition, 
	float3 fragmentNormal,
	float3 viewWorldPosition
)
{
	float3 lightColourTotal = {0.f, 0.f, 0.f};
	float3 ambientLighting = {0.f, 0.f, 0.f}; 
	float3 pointLighting = {0.f, 0.f, 0.f};
	float3 directionalLighting = {0.f, 0.f, 0.f};
	float3 lightVector;
	int totalNPointLights = 0;
	int totalNDirectionalLights = 0;
	int i;


	if ((bool) ambientLightData.isEnabled)
	{
		ambientLighting = calculateAmbientLighting(ambientLightData.colour.rgb);
	}
	lightColourTotal += ambientLighting;

	PointLightBufferType firstPointLightData = PointLightBuffer[0];
	totalNPointLights = firstPointLightData.nLights.x;
	for(i=0; i<totalNPointLights; i++)
	{
		float3 lightColourContribution;
		PointLightBufferType pointLightData = PointLightBuffer[i];
		lightVector = normalize(pointLightData.worldPosition.xyz - fragmentWorldPosition.xyz).xyz;

		lightColourContribution = calculatePointLighting(
			pointLightData,
			fragmentWorldPosition, 
			fragmentNormal, 
			lightVector,
			viewWorldPosition
		);
		lightColourTotal += lightColourContribution;
	}

	DirectionalLightBufferType firstDirectionalLightData = DirectionalLightBuffer[0];
	totalNDirectionalLights = firstDirectionalLightData.nLights.x;
	for(i=0; i<totalNDirectionalLights; i++)
	{
		float3 lightColourContribution;
		DirectionalLightBufferType directionalLightData = DirectionalLightBuffer[i];

		lightColourContribution = calculateDirectionalLighting(
			directionalLightData,
			fragmentWorldPosition,
			fragmentNormal,
			viewWorldPosition
		);
		lightColourTotal += lightColourContribution;
	}

	return lightColourTotal;
}

float4 getTextureColour(Texture2D _objectTexture, SamplerState _textureSampler, float2 uv)
{
	return _objectTexture.Sample(_textureSampler, uv);
}


float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour = 0.f;
	textureColour = getTextureColour(objectTexture, textureSampler, input.tex);
	lightColour = calculateLighting(input.worldPosition.xyz, input.normal.xyz, input.viewVector.xyz).rgbb;
	lightColour[3] = 1.f;
	
    return textureColour * lightColour;
}
