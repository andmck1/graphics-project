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

OutputType main(InputType input)
{
	OutputType output;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
	
    float4 worldPosition = mul(input.position, worldMatrix);
    output.worldPosition = worldPosition.xyz;
	
    output.viewVector = normalize(cameraData.worldPosition - worldPosition.xyz);

	return output;
}
