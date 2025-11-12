#include "sharedbuffertypes.h"

// Note, code provided from lab examples
cbuffer MatrixBuffer : register(b0)
{
    MatrixBufferType matrixData;
};

DepthVertexOutputType main(VertexInputType input)
{
    DepthVertexOutputType output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);

    // Store the position value in a second input value for depth value calculations.
    output.depthPosition = output.position;
	
    return output;
}
