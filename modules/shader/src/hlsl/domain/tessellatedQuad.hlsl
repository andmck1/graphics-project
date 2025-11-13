// Lab code
// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
#include "sharedbuffertypes.h"
cbuffer MatrixBuffer : register(b0)
{
    MatrixDomainBufferType matrixData;
};

[domain("quad")]
QuadTessellationDomainOutputType main(
    QuadTessellationConstantOutputType input, 
    float2 uv: SV_DomainLocation, 
    const OutputPatch<QuadTessellationInputType, 4> patch
)
{
    QuadTessellationDomainOutputType output;
    float3 v1, v2, vertexPosition;

   // Frank Luna uses a bilinear interpolation here, and was told to use it in lecture for quad
    v1 = lerp(patch[0].position, patch[1].position, 1.f - uv.y);
    v2 = lerp(patch[3].position, patch[2].position, 1.f - uv.y);
    vertexPosition = lerp(v1, v2, uv.x);
    
    // Displacement mapping code would go here

    output.position = mul(float4(vertexPosition, 1.0f), matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);

    // Send the input color into the pixel shader.
    output.colour = patch[0].colour;

    return output;
}


