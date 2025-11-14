#include "sharedbuffertypes.h"

Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);
Texture2D shadowMapTexture1: register(t1);
Texture2D shadowMapTexture2: register(t2);

cbuffer AmbientLightBuffer: register(b0)
{
	AmbientLightBufferType ambientLightData;
}

cbuffer LightSettingsBuffer: register(b1)
{
	LightSettingsBufferType lightSettings;
}

StructuredBuffer<DirectionalLightBufferType> DirectionalLightBuffer : register(t3);
StructuredBuffer<PointLightBufferType> PointLightBuffer : register(t4);
StructuredBuffer<SpotLightBufferType> SpotLightBuffer : register(t5);

// Lab code
bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

// Lab code
bool isInShadow(
	Texture2D _shadowMapTexture, SamplerState _sampler, 
	float2 _uv, float4 lightViewPosition, float bias
)
{
    float depthValue = _shadowMapTexture.Sample(_sampler, _uv).r;
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;

    if (lightDepthValue < depthValue)
    {
        return false;
    }
    return true;
}

// Lab code
float2 getProjectiveCoords(float4 lightViewPosition)
{
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}


float GetHeight(float2 uv)
{
    float4 textureColour = objectTexture.SampleLevel(textureSampler, uv, 0);
    return textureColour.r * 500;
}

// Lab 5 code
// Use neighbouring texels to calculate surface normals
float3 calculateNormalUsingNeighbouringTexels(float2 uv, float3 posCentre)
{
    uint height;
    uint width;
    
    // Calculate uv coordinate step lengths as a function of texture resolution
    objectTexture.GetDimensions(width, height);
    float uvStepResolution = 1.0f;
    float uvStepX = uvStepResolution * 1.0f / width;
    float uvStepY = uvStepResolution * 1.0f / height;
    
    // Heights from height map of centre and two neighbours
    float heightCentre = GetHeight(uv);
    float heightWorldScaling = posCentre.y / heightCentre;
    float heightNorth = GetHeight(uv + float2(0.0f, uvStepY));
    float heightWest = GetHeight(uv - float2(uvStepX, 0.0f));
    float heightSouth = GetHeight(uv - float2(0.0f, uvStepY));
    float heightEast = GetHeight(uv + float2(uvStepX, 0.0f));
    
    // Position vectors
    float3 posCentreUV = float3(uv.x * width, posCentre.y, uv.y * height);
    float3 posNorthUV = float3(uv.x * width, heightNorth * heightWorldScaling, (uv.y + uvStepY) * height);
    float3 posWestUV = float3((uv.x - uvStepX) * width, heightWest * heightWorldScaling, uv.y * height);
    float3 posSouthUV = float3(uv.x * width, heightSouth * heightWorldScaling, (uv.y - uvStepY) * height);
    float3 posEastUV = float3((uv.x + uvStepX) * width, heightEast * heightWorldScaling, uv.y * height);
    
    // Tangent and bitangent
    float3 tangent = posCentreUV - posNorthUV;
    float3 bitangent = posCentreUV - posWestUV;
    float3 tangent2 = posCentreUV - posSouthUV;
    float3 bitangent2 = posCentreUV - posEastUV;
    
    // Cross product
    float3 normal = normalize(cross(tangent, bitangent)) + normalize(cross(bitangent, tangent2)) + normalize(cross(tangent2, bitangent2)) + normalize(cross(bitangent2, tangent));
    
    return -normal * 0.25;
}

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

float3 calculateSpotLighting(
	SpotLightBufferType spotLightData,
	float3 fragmentWorldPosition, 
	float3 fragmentNormal, 
	float3 lightVector,
	float3 viewWorldPosition
)
{
	float3 spotLighting;
	float3 diffuse = {0.f, 0.f, 0.f};
	float3 specular = {0.f, 0.f, 0.f};
	float distance;
	float attenuation = 1.f;
	float directionalAttenuation;

	if ((bool) spotLightData.isDiffuseEnabled.r)
	{
		diffuse = calculateDiffuseLighting(
			spotLightData.diffuseColour.rgb, 
			fragmentNormal, 
			lightVector
		);
	}
	if ((bool) spotLightData.isSpecularEnabled.r)
	{
		specular = calculateSpecularLighting(
			spotLightData.specularColour.rgb,
			spotLightData.specularPower.r,
			viewWorldPosition,
			lightVector,
			fragmentNormal	
		);
	}

	if ((bool) spotLightData.isAttenuationEnabled.r)
	{
		distance = length(spotLightData.worldPosition.xyz - fragmentWorldPosition);
		attenuation = 1.0f / (
			spotLightData.attenuationCoefficients[0] + 
			(spotLightData.attenuationCoefficients[1] * distance) + 
			(spotLightData.attenuationCoefficients[2] * distance * distance)
		);
	}

	spotLighting = diffuse + specular;
	spotLighting *= attenuation;

	directionalAttenuation = dot(lightVector, -normalize(spotLightData.directionVector.xyz));
	if (directionalAttenuation < cos(spotLightData.coneAngle.x * (3.14159f / 180.f)))
	{
		directionalAttenuation = 0.f;
	}
	spotLighting *= directionalAttenuation;

	return spotLighting;
}

float3 calculateAmbientLighting(float3 colour)
{
	return max(colour, float3(0.f, 0.f, 0.f));
}

float3 calculateLighting(
	float3 fragmentWorldPosition, 
	float3 fragmentNormal,
	float3 viewWorldPosition,
	float4 spotLightviewPos1,
	float4 spotLightviewPos2,
	Texture2D _shadowMapTexture1,
	Texture2D _shadowMapTexture2,
	SamplerState _shadowSampler
)
{
	float3 lightColourTotal = {0.f, 0.f, 0.f};
	float3 ambientLighting = {0.f, 0.f, 0.f}; 
	float shadowMapBias = 0.005f;
	float3 lightVector;
	float2 projectedTextureCoordinate;
	int totalNPointLights = 0;
	int totalNSpotLights = 0;
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

	projectedTextureCoordinate = getProjectiveCoords(spotLightviewPos1);
	if (hasDepthData(projectedTextureCoordinate))
	{
		if (!isInShadow(
			_shadowMapTexture1,
			_shadowSampler,
			projectedTextureCoordinate,
			spotLightviewPos1,
			shadowMapBias
		))
		{
			SpotLightBufferType spotLightData = SpotLightBuffer[0];
			lightVector = normalize(spotLightData.worldPosition.xyz - fragmentWorldPosition.xyz).xyz;
			lightColourTotal += calculateSpotLighting(
				spotLightData,
				fragmentWorldPosition, 
				fragmentNormal, 
				lightVector,
				viewWorldPosition
			);
		}
	}

	projectedTextureCoordinate = getProjectiveCoords(spotLightviewPos2);
	if (hasDepthData(projectedTextureCoordinate))
	{
		if (!isInShadow(
			_shadowMapTexture2,
			_shadowSampler,
			projectedTextureCoordinate,
			spotLightviewPos2,
			shadowMapBias
		))
		{
			SpotLightBufferType spotLightData = SpotLightBuffer[1];
			lightVector = normalize(spotLightData.worldPosition.xyz - fragmentWorldPosition.xyz).xyz;
			lightColourTotal += calculateSpotLighting(
				spotLightData,
				fragmentWorldPosition, 
				fragmentNormal, 
				lightVector,
				viewWorldPosition
			);
		}
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

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColour;
	float4 lightColour = {0.f, 0.f, 0.f, 0.f};
	float2 projectedTextureCoordinate;
	textureColour = getTextureColour(objectTexture, textureSampler, input.tex);

	if ((bool) lightSettings.isNormalsAsColourEnabled.x)
	{
		return float4((input.normal + 1.f) / 2.f, 1.f);
	}
	lightColour += calculateLighting(
		input.worldPosition.xyz, input.normal.xyz, input.viewVector.xyz,
		input.spotLightViewPos1, input.spotLightViewPos2,
		shadowMapTexture1, shadowMapTexture2, textureSampler
	).rgbb;
	lightColour[3] = 1.f;
	
    return textureColour * lightColour;
}
