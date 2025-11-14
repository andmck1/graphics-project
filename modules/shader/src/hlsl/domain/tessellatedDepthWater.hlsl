Texture2D heightMap : register(t0);
SamplerState heightSampler : register(s0);

#include "sharedbuffertypes.h"
cbuffer MatrixBuffer : register(b0)
{
    MatrixDomainBufferType matrixData;
};

cbuffer WaveBuffer : register(b1)
{
    WaveDataBufferType waveData;
}

// TODO - a lot of AI help here to shoe horn this in. Need to go back and understand it
float3 getWaveDisplacement(float4 position, WaveDataBufferType w)
{
    float3 disp = float3(position.x, position.y, position.z);

    float A[3] = { w.magnitude.x, w.magnitude.y, w.magnitude.z };
    float k[3] = { w.waveNumber.x, w.waveNumber.y, w.waveNumber.z };
    float wv[3] = { w.angularVelocity.x, w.angularVelocity.y, w.angularVelocity.z };
    float2 D[3] = {
        normalize(float2( 1.f,  0.f)),
        normalize(float2( 0.6f, 0.8f)),
        normalize(float2(-0.7f, 0.3f))
    };

    float Q = w.steepness.x;    // steepness
    float t = w.totalTime.x;

    for (int i = 0; i < 3; i++)
    {
        float theta = dot(D[i] * k[i], position.xz) + wv[i] * t;
        disp.x += Q * A[i] * D[i].x * cos(theta);
        disp.z += Q * A[i] * D[i].y * cos(theta);
        disp.y +=       A[i]         * sin(theta);
    }

    return disp;
}

float3 getWaveNormal(float4 position, WaveDataBufferType w)
{
    float3 n = float3(0.f, 1.f, 0.f);

    float A[3] = { w.magnitude.x, w.magnitude.y, w.magnitude.z };
    float k[3] = { w.waveNumber.x, w.waveNumber.y, w.waveNumber.z };
    float wv[3] = { w.angularVelocity.x, w.angularVelocity.y, w.angularVelocity.z };
    float2 D[3] = {
        normalize(float2( 1.f,  0.f)),
        normalize(float2( 0.6f, 0.8f)),
        normalize(float2(-0.7f, 0.3f))
    };

    float Q = w.steepness.x;
    float t = w.totalTime.x;

    float3 accum = float3(0, 0, 0);

    for (int i = 0; i < 3; i++)
    {
        float theta = dot(D[i] * k[i], position.xz) + wv[i] * t;
        float costheta = cos(theta);
        float sintheta = sin(theta);

        accum.x += -D[i].x * k[i] * A[i] * costheta;
        accum.z += -D[i].y * k[i] * A[i] * costheta;
        accum.y += 1.0 - Q * k[i] * A[i] * sintheta;
    }

    return normalize(accum);
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
    float waveDisplacement;
    float3 waveNormal;

    // Frank Luna uses a bilinear interpolation here, and was told to use it in lecture for quad
    v1 = lerp(patch[0].position, patch[1].position, uvPatch.x);
    v2 = lerp(patch[3].position, patch[2].position, uvPatch.x);
    vertexPosition3f = lerp(v1, v2, uvPatch.y);
    vertexPosition4f = float4(vertexPosition3f, 1.f);

    // n1 = lerp(patch[0].normal, patch[1].normal, uvPatch.x);
    // n2 = lerp(patch[3].normal, patch[2].normal, uvPatch.x);
    // normal = normalize(lerp(n1, n2, uvPatch.y));

    uv1 = lerp(patch[0].tex, patch[1].tex, uvPatch.x);
    uv2 = lerp(patch[3].tex, patch[2].tex, uvPatch.x);
    uv = lerp(uv1, uv2, uvPatch.y); 
    
	// WAVE
    float3 displacedVertex = getWaveDisplacement(vertexPosition4f, waveData);
    waveNormal = getWaveNormal(vertexPosition4f, waveData);
    vertexPosition4f.xyz = displacedVertex;
    normal = normalize(waveNormal);
    // vertexPosition4f.xyz += normal.xyz;	

    output.position = mul(vertexPosition4f, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);

    output.depthPosition = output.position;
 
    return output;
}
