Texture2D heightMap : register(t0);
SamplerState heightSampler : register(s0);

#include "sharedbuffertypes.h"
cbuffer MatrixBuffer : register(b0)
{
    MatrixDomainBufferType matrixData;
};

cbuffer CameraBuffer : register(b1)
{
	CameraBufferType cameraData;
};

cbuffer WaveBuffer : register(b2)
{
    WaveDataBufferType waveData;
}

[domain("quad")]
QuadTessellationDomainOutputType main(
    QuadTessellationConstantOutputType input, 
    float2 uv: SV_DomainLocation, 
    const OutputPatch<QuadTessellationHullOutputType, 4> patch
)
{
    QuadTessellationDomainOutputType output;
    float3 v1, v2, vertexPosition, n1, n2, normal;

    // Frank Luna uses a bilinear interpolation here, and was told to use it in lecture for quad
    v1 = lerp(patch[0].position, patch[1].position, 1.f - uv.y);
    v2 = lerp(patch[3].position, patch[2].position, 1.f - uv.y);
    vertexPosition = lerp(v1, v2, uv.x);

    n1 = lerp(patch[0].normal, patch[1].normal, 1.f - uv.y);
    n2 = lerp(patch[3].normal, patch[2].normal, 1.f - uv.y);
    normal = normalize(lerp(n1, n2, uv.x));
    
    // Displacement mapping code would go here


    output.position = mul(float4(vertexPosition, 1.0f), matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);

    output.tex = uv;

    output.normal = normal;

    float4 worldPosition = mul(float4( vertexPosition, 1.f), matrixData.worldMatrix);
    output.worldPosition = worldPosition.xyz;
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

    // output.spotLightViewPos1 = mul(input.position, matrixData.worldMatrix);
    // output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightViewMatrix1);
    // output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightProjectionMatrix1);
    // output.spotLightViewPos2 = mul(input.position, matrixData.worldMatrix);
    // output.spotLightViewPos2 = mul(output.spotLightViewPos2, matrixData.spotLightViewMatrix2);
    // output.spotLightViewPos2 = mul(output.spotLightViewPos2, matrixData.spotLightProjectionMatrix2);
    output.spotLightViewPos1 = float4(1.f, 2.f, 3.f, 1.f) ;
    output.spotLightViewPos2 = float4(1.f, 2.f, 3.f, 1.f) ;
 
    return output;
}


