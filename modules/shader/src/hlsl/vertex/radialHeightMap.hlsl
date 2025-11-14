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

// TODO - repeated code, should be in a shared shader file
float getHeight(float2 _uv, Texture2D _heightMap, SamplerState _heightMapSampler)
{
    float4 textureColour;
    // Prevent from sampling around the seam of the texture
    _uv.x = frac(_uv.x + 1e-6);
    textureColour = _heightMap.SampleLevel(_heightMapSampler, _uv, 0);
    // TODO - parameterise
    float height = 1.f;
    return (textureColour.r) * height;

}

VertexOutputType main(VertexInputType input)
{
    VertexOutputType output;

	// TERRAIN HEIGHT
    float4 displacedPosition;
    displacedPosition = input.position;
    displacedPosition.xyz += input.normal * getHeight(input.tex, heightMap, heightSampler);
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(displacedPosition, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);
	
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Normal
    output.normal = input.normal;
    
	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(output.normal, (float3x3) matrixData.worldMatrix);
    output.normal = normalize(output.normal);

    float4 worldPosition = mul(input.position, matrixData.worldMatrix);
    output.worldPosition = worldPosition.xyz;
	
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

    // Lab code
    output.spotLightViewPos1 = mul(input.position, matrixData.worldMatrix);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightViewMatrix1);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightProjectionMatrix1);
    output.spotLightViewPos2 = mul(input.position, matrixData.worldMatrix);
    output.spotLightViewPos2 = mul(output.spotLightViewPos2, matrixData.spotLightViewMatrix2);
    output.spotLightViewPos2 = mul(output.spotLightViewPos2, matrixData.spotLightProjectionMatrix2);

    return output;
}
