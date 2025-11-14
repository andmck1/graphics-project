Texture2D heightMap : register(t0);
SamplerState heightSampler : register(s0);

#include "sharedbuffertypes.h"
cbuffer MatrixBuffer : register(b0)
{
    MatrixDomainBufferType matrixData;
};

float GetHeight(float2 _uv, Texture2D _heightMap, SamplerState _heightMapSampler)
{
    float4 textureColour;
    textureColour = _heightMap.SampleLevel(_heightMapSampler, _uv, 0);
    // TODO - parameterise
    float height = 50.f;
    float offsetFactor = 0.25f;
    return (textureColour.r * 2 - 1) * height + (height*offsetFactor);
}

[domain("quad")]
DepthQuadTessellationDomainOutputType main(
    QuadTessellationConstantOutputType input, 
    float2 uvPatch: SV_DomainLocation, 
    const OutputPatch<QuadTessellationHullOutputType, 4> patch
)
{
    DepthQuadTessellationDomainOutputType output;
    float3 v1, v2, vertexPosition3f, n1, n2, normal;
    float2 uv1, uv2, uv;
    float4 vertexPosition4f;
    float heightDisplacement;

    // Frank Luna uses a bilinear interpolation here, and was told to use it in lecture for quad
    v1 = lerp(patch[0].position, patch[1].position, uvPatch.x);
    v2 = lerp(patch[3].position, patch[2].position, uvPatch.x);
    vertexPosition3f = lerp(v1, v2, uvPatch.y);
    vertexPosition4f = float4(vertexPosition3f, 1.f);

    n1 = lerp(patch[0].normal, patch[1].normal, uvPatch.x);
    n2 = lerp(patch[3].normal, patch[2].normal, uvPatch.x);
    normal = normalize(lerp(n1, n2, uvPatch.y));

    uv1 = lerp(patch[0].tex, patch[1].tex, uvPatch.x);
    uv2 = lerp(patch[3].tex, patch[2].tex, uvPatch.x);
    uv = lerp(uv1, uv2, uvPatch.y); 

    heightDisplacement = GetHeight(uv, heightMap, heightSampler);
    vertexPosition3f.y += heightDisplacement;
    vertexPosition4f.y += heightDisplacement;

    output.position = mul(vertexPosition4f, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);
    output.depthPosition = output.position;
 
    return output;
}


