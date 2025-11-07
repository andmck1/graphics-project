#include "standardVertexBuffer.h"
#include "waveBuffer.h"


// Note, code provided from lab examples
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
	CameraBufferType cameraData;
};

cbuffer WaveBuffer : register(b2)
{
	WaveDataBufferType waveData;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};

OutputType main(InputType input)
{
	OutputType output;

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
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
	
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	output.normal = input.normal;
    
	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(output.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
	
    float4 worldPosition = mul(input.position, worldMatrix);
    output.worldPosition = worldPosition.xyz;
	
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

	return output;
}
