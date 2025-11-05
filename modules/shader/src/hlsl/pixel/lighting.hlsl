#include "lightingBuffer.h"

Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);

cbuffer AmbientLightBuffer: register(b0)
{
	AmbientLightBuffer ambientLightData;
}

cbuffer PointLightBuffer : register(b1)
{
	PointLightBuffer pointLightData;
}

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
    float diffuseIntensity = dot(normal, lightVector);
    diffuseLighting = diffuseColour * diffuseIntensity;
	return diffuseLighting;
}

float3 calculatePointLighting(
	float3 diffuseLightColour, 
	float3 specularLightColour, 
	float specularLightPower,
	float3 pointLightWorldPosition, 
	float3 pointLightAttenuationCoefficients,
	bool isDiffuseEnabled,
	bool isSpecularEnabled,
	bool isAttenuationEnabled,
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

	if (isDiffuseEnabled)
	{
		diffuse = calculateDiffuseLighting(
			diffuseLightColour, 
			fragmentNormal, 
			lightVector
		);
	}
	if (isSpecularEnabled)
	{
		specular = calculateSpecularLighting(
			specularLightColour,
			specularLightPower,
			viewWorldPosition,
			lightVector,
			fragmentNormal	
		);
	}

	if (isAttenuationEnabled)
	{
		distance = length(pointLightWorldPosition - fragmentWorldPosition);
		attenuation = 1.0f / (
			pointLightAttenuationCoefficients[0] + 
			(pointLightAttenuationCoefficients[1] * distance) + 
			(pointLightAttenuationCoefficients[2] * distance * distance)
		);
	}

	pointLighting = diffuse + specular;
	pointLighting *= attenuation;
	return pointLighting;
}

float3 calculateAmbientLighting(float3 colour)
{
	return colour;
}

float3 calculateLighting(
	float3 ambientColour, 
	bool isAmbientEnabled,
	float3 pointDiffuseLightColour,
	float3 pointSpecularLightColour, 
	float pointSpecularLightPower,
	float3 pointLightWorldPosition, 
	float3 pointLightAttenuationCoefficients,
	bool isPointLightDiffuseEnabled,
	bool isPointLightSpecularEnabled,
	bool isPointLightAttenuationEnabled,
	float3 fragmentWorldPosition, 
	float3 fragmentNormal,
	float3 viewWorldPosition
)
{
	float3 calculatedLightColour;
	float3 ambientLighting = {0.f, 0.f, 0.f}; 
	float3 pointLighting;
	float3 lightVector;

	lightVector = normalize(pointLightWorldPosition - fragmentWorldPosition);

	if (isAmbientEnabled)
	{
		ambientLighting = calculateAmbientLighting(ambientColour);
	}
	pointLighting = calculatePointLighting(
		pointDiffuseLightColour, 
		pointSpecularLightColour, 
		pointSpecularLightPower,
		pointLightWorldPosition, 
		pointLightAttenuationCoefficients,
		isPointLightDiffuseEnabled,
		isPointLightSpecularEnabled,
		isPointLightAttenuationEnabled,
		fragmentWorldPosition, 
		fragmentNormal, 
		lightVector,
		viewWorldPosition
	);

	calculatedLightColour = saturate(ambientLighting + pointLighting);
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
	lightColour = calculateLighting(
		ambientLightData.colour.rgb,
		ambientLightData.isEnabled.r,
		pointLightData.diffuseColour.rgb,
		pointLightData.specularColour.rgb,
		pointLightData.specularPower.r,
		pointLightData.worldPosition.xyz,
		pointLightData.attenuationCoefficients.xyz,
		pointLightData.isDiffuseEnabled.r,
		pointLightData.isSpecularEnabled.r,
		pointLightData.isAttenuationEnabled.r,
		input.worldPosition,
		input.normal,
		input.viewVector
	).rgbb;
	lightColour[3] = 1.f;
	
    return textureColour * lightColour;
}
