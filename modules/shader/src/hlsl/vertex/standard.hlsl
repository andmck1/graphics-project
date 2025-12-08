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

    // Lab code
    output.spotLightViewPos0 = mul(input.position, matrixData.worldMatrix);
    output.spotLightViewPos0 = mul(output.spotLightViewPos0, matrixData.spotLightViewMatrix0);
    output.spotLightViewPos0 = mul(output.spotLightViewPos0, matrixData.spotLightProjectionMatrix0);
    output.spotLightViewPos1 = mul(input.position, matrixData.worldMatrix);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightViewMatrix1);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightProjectionMatrix1);
    output.directionalLightViewPos0 = mul(input.position, matrixData.worldMatrix);
    output.directionalLightViewPos0 = mul(output.directionalLightViewPos0, matrixData.directionalLightViewMatrix0);
    output.directionalLightViewPos0 = mul(output.directionalLightViewPos0, matrixData.directionalLightProjectionMatrix0);
    output.directionalLightViewPos1 = mul(input.position, matrixData.worldMatrix);
    output.directionalLightViewPos1 = mul(output.directionalLightViewPos1, matrixData.directionalLightViewMatrix1);
    output.directionalLightViewPos1 = mul(output.directionalLightViewPos1, matrixData.directionalLightProjectionMatrix1);

	return output;
}
