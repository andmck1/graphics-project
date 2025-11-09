#include "sharedbuffertypes.h"

// My code from lab 5
Texture2D heightMap : register(t0);
SamplerState heightSampler : register(s0);

cbuffer MatrixBuffer : register(b0)
{
    MatrixBufferType matrixData;
};

cbuffer CameraBuffer : register(b1)
{
	CameraBufferType cameraData;
};

// TODO - define these structs in buffer.h

float GetHeight(float2 uv)
{
    float4 textureColour = heightMap.SampleLevel(heightSampler, uv, 0);
    return textureColour.r * 500 - 250;
}

// Calculate normal using tangent and bitangent from two adjacent vertices
float3 CalculateTangentBitangetNormal(float2 uv, float3 posCentre)
{
    // Our vertex density (square 100x100 vertex plane)
    float uvStep = 0.01;
    
    // Heights from height map of centre and two neighbours
    float heightCentre = GetHeight(uv);
    float heightNorth = GetHeight(uv + float2(0.0f, uvStep));
    float heightWest = GetHeight(uv - float2(uvStep, 0.0f));
    
    // Position vectors
    float3 posNorth = float3(posCentre.x, heightNorth, posCentre.y + 1);
    float3 posWest = float3(posCentre.x - 1, heightWest, posCentre.y);
    
    // Tangent and bitangent
    float3 tangent = posCentre - posNorth;
    float3 bitangent = posCentre - posWest;
    
    // Cross product
    float3 normal = normalize(cross(tangent, bitangent));
    
    return normal;
}

VertexOutputType main(VertexInputType input)
{
    VertexOutputType output;

	// TERRAIN HEIGHT
    float4 displacedPosition = input.position.xyzz;
    displacedPosition.w = 1.f;
    displacedPosition.y += GetHeight(input.tex);
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(displacedPosition, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);
	
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the terrain normal based on surrounding neighbours
    // output.normal = CalculateAverageSharedNormal(input.tex);
    output.normal = CalculateTangentBitangetNormal(input.tex, displacedPosition.xyz);
    
	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(output.normal, (float3x3) matrixData.worldMatrix);
    output.normal = normalize(output.normal);

    float4 worldPosition = mul(input.position, matrixData.worldMatrix);
    output.worldPosition = worldPosition.xyz;
	
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

    return output;
}
