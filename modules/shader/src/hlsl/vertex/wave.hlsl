#include "sharedbuffertypes.h"

// Note, code provided from lab examples
cbuffer MatrixBuffer : register(b0)
{
    MatrixBufferType matrixData;
};

cbuffer CameraBuffer : register(b1)
{
	CameraBufferType cameraData;
};

cbuffer WaveBuffer : register(b2)
{
	WaveDataBufferType waveData;
};

VertexOutputType main(VertexInputType input)
{
	VertexOutputType output;

	// WAVE
	// Move wave through position
	// Move the vertex along y axis
    input.position.y += (
		(
			waveData.magnitude.x * 
			sin(
				(waveData.waveNumber.x * input.position.x) - 
				(waveData.angularVelocity.x * waveData.totalTime.x)
			)
		) + 
		(
			waveData.magnitude.x * 
			cos(
				(waveData.waveNumber.x * input.position.z) - 
				(waveData.angularVelocity.x * waveData.totalTime.x)
				)
		)
	);
	
	// Normals
	// Wave Normals
    input.normal.x = (
		-waveData.magnitude.x * 
		waveData.waveNumber.x * 
		cos(
			(waveData.waveNumber.x * input.position.x)  - 
			(waveData.angularVelocity.x * waveData.totalTime.x)
		)
	);
    input.normal.y = 1.0f;
    input.normal.z = (
		waveData.magnitude.x * 
		waveData.waveNumber.x * 
		sin(
			(waveData.waveNumber.x * input.position.z) - 
			(waveData.angularVelocity.x * waveData.totalTime.x)
		)
	);
	
	// Move the vertex along the normal
    input.position.xyz += input.normal.xyz;	

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);
	
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	output.normal = input.normal;
    
	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(output.normal, (float3x3) matrixData.worldMatrix);
    output.normal = normalize(output.normal);
	
    float4 worldPosition = mul(input.position, matrixData.worldMatrix);
    output.worldPosition = worldPosition.xyz;
	
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

	return output;
}

