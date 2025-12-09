Texture2D heightMap : register(t0);
SamplerState heightSampler : register(s0);

#include "sharedbuffertypes.h"
cbuffer MatrixBuffer : register(b0)
{
    MatrixDomainBufferType matrixData;
};

cbuffer CameraBuffer : register(b1)
{
	CameraDomainBufferType cameraData;
};

cbuffer WaveBuffer : register(b2)
{
    WaveDataBufferType waveData;
}

// wave derivation depends on this so don't touch!
static const int numWaves = 2;

struct WaveResult
{
    float3 position;
    float3 normal;
    float3 binormal;
    float3 tangent;
};
 
struct Wave
{
    float3 direction;
    float steepness;
    float waveLength;
    float amplitude;
    float speed;
};
 
WaveResult CalculateWave(Wave wave, float3 wavePosition, float edgeDampen, float time)
{
    WaveResult result;
 
    float frequency = 2.0 / wave.waveLength;
    float phaseConstant = wave.speed * frequency;
    float qi = wave.steepness / (wave.amplitude * frequency * numWaves);
    float rad = frequency * dot(wave.direction.xz, wavePosition.xz) + time * phaseConstant;
    float sinR = sin(rad);
    float cosR = cos(rad);
 
    result.position.x = wavePosition.x + qi * wave.amplitude * wave.direction.x * cosR * edgeDampen;
    result.position.z = wavePosition.z + qi * wave.amplitude * wave.direction.z * cosR * edgeDampen;
    result.position.y = wave.amplitude * sinR * edgeDampen;
 
    float waFactor = frequency * wave.amplitude;
    float radN = frequency * dot(wave.direction, result.position) + time * phaseConstant;
    float sinN = sin(radN);
    float cosN = cos(radN);
 
    result.binormal.x = 1 - (qi * wave.direction.x * wave.direction.x * waFactor * sinN);
    result.binormal.z = -1 * (qi * wave.direction.x * wave.direction.z * waFactor * sinN);
    result.binormal.y = wave.direction.x * waFactor * cosN;
 
    result.tangent.x = -1 * (qi * wave.direction.x * wave.direction.z * waFactor * sinN);
    result.tangent.z = 1 - (qi * wave.direction.z * wave.direction.z * waFactor * sinN);
    result.tangent.y = wave.direction.z * waFactor * cosN;
 
    result.normal.x = -1 * (wave.direction.x * waFactor * cosN);
    result.normal.z = -1 * (wave.direction.z * waFactor * cosN);
    result.normal.y = 1 - (qi * waFactor * sinN);
 
    result.binormal = normalize(result.binormal);
    result.tangent = normalize(result.tangent);
    result.normal = normalize(result.normal);
 
    return result;
}


[domain("quad")]
QuadTessellationDomainOutputType main(
    QuadTessellationConstantOutputType input, 
    float2 uvPatch: SV_DomainLocation, 
    const OutputPatch<QuadTessellationHullOutputType, 4> patch
)
{
    QuadTessellationDomainOutputType output;
    float2 uv1, uv2, uv;
    float3 v1, v2, n1, n2, normal;
    float4 vertexPosition;

    // Frank Luna uses a bilinear interpolation here, and was told to use it in lecture for quad
    v1 = lerp(patch[0].position, patch[1].position, uvPatch.x);
    v2 = lerp(patch[3].position, patch[2].position, uvPatch.x);
    vertexPosition =float4( lerp(v1, v2, uvPatch.y), 1.f);

    uv1 = lerp(patch[0].tex, patch[1].tex, uvPatch.x);
    uv2 = lerp(patch[3].tex, patch[2].tex, uvPatch.x);
    uv = lerp(uv1, uv2, uvPatch.y); 
    
	// WAVE
    float dampeningFactor = 5.f;
    Wave waves[numWaves];
    waves[0].direction = float3(0.3, 0, -0.7);
    waves[0].steepness = 1.79;
    waves[0].waveLength = 3.75;
    waves[0].amplitude = 0.85;
    waves[0].speed = 1.21;
 
    waves[1].direction = float3(0.5, 0, -0.2);
    waves[1].steepness = 1.79;
    waves[1].waveLength = 4.1;
    waves[1].amplitude = 0.52;
    waves[1].speed = 1.03;
 
    float dampening = 1.0 - pow(saturate(abs(uv.x - 0.5) / 0.5), dampeningFactor);
    dampening *= 1.0 - pow(saturate(abs(uv.y - 0.5) / 0.5), dampeningFactor);
 
    WaveResult finalWaveResult;
    finalWaveResult.position = float3(0,0,0);
    finalWaveResult.normal = float3(0,0,0);
    finalWaveResult.tangent = float3(0,0,0);
    finalWaveResult.binormal = float3(0,0,0);
 
    for(int waveId = 0; waveId < numWaves; waveId++)
    {
        WaveResult waveResult = CalculateWave(waves[waveId], vertexPosition.xyz, dampening, waveData.totalTime.x);
        finalWaveResult.position += waveResult.position;
        finalWaveResult.normal += waveResult.normal;
        finalWaveResult.tangent += waveResult.tangent;
        finalWaveResult.binormal += waveResult.binormal;
    }
 
    finalWaveResult.position -= vertexPosition.xyz * (numWaves - 1);
    finalWaveResult.normal = normalize(finalWaveResult.normal);
    finalWaveResult.tangent = normalize(finalWaveResult.tangent);
    finalWaveResult.binormal = normalize(finalWaveResult.binormal);

    vertexPosition = float4(finalWaveResult.position, 1.f);
    normal = normalize(mul(finalWaveResult.normal, (float3x3)matrixData.worldMatrix));

    output.position = mul(vertexPosition, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);

    output.tex = uv * 10.f;

    output.normal = normal;

    float4 worldPosition = mul(vertexPosition, matrixData.worldMatrix);
    output.worldPosition = worldPosition.xyz;
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

    output.spotLightViewPos0 = mul(vertexPosition, matrixData.worldMatrix);
    output.spotLightViewPos0 = mul(output.spotLightViewPos0, matrixData.spotLightViewMatrix0);
    output.spotLightViewPos0 = mul(output.spotLightViewPos0, matrixData.spotLightProjectionMatrix0);
    output.spotLightViewPos1 = mul(vertexPosition, matrixData.worldMatrix);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightViewMatrix1);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightProjectionMatrix1);
    output.directionalLightViewPos0 = mul(vertexPosition, matrixData.worldMatrix);
    output.directionalLightViewPos0 = mul(output.directionalLightViewPos0, matrixData.directionalLightViewMatrix0);
    output.directionalLightViewPos0 = mul(output.directionalLightViewPos0, matrixData.directionalLightProjectionMatrix0);
    output.directionalLightViewPos1 = mul(vertexPosition, matrixData.worldMatrix);
    output.directionalLightViewPos1 = mul(output.directionalLightViewPos1, matrixData.directionalLightViewMatrix1);
    output.directionalLightViewPos1 = mul(output.directionalLightViewPos1, matrixData.directionalLightProjectionMatrix1);

    output.depthPosition = output.position;
 
    return output;
}
