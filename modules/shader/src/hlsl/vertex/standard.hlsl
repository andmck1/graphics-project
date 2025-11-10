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

VertexOutputType main(VertexInputType input)
{
	VertexOutputType output;

	output.position = mul(input.position, matrixData.worldMatrix);
	output.position = mul(output.position, matrixData.viewMatrix);
	output.position = mul(output.position, matrixData.projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)matrixData.worldMatrix);
	output.normal = normalize(output.normal);
	
    float4 worldPosition = mul(input.position, matrixData.worldMatrix);
    output.worldPosition = worldPosition.xyz;
	
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

	return output;
}
