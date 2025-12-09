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

// Don't touch!
static const int numWaves = 2;

struct WaveResult
{
    float3 position;
    float3 normal;
    float3 binormal;
    float3 tangent;
};

// (Tardif, n.d.)
WaveResult CalculateWave(WaveDataBufferType waveData, int waveIndex, float3 wavePosition, float edgeDampen, float time)
{
    WaveResult result;
    
    // get data
    float3 direction = waveData.direction[waveIndex].xyz;
    float magnitude = waveData.magnitude[waveIndex].x;
    float angularVelocity = waveData.angularVelocity[waveIndex].x;
    float waveNumber = waveData.waveNumber[waveIndex].x;
    float steepness = waveData.steepness[waveIndex].x;
    
    // calculate useful values
    float wavelength = 1.f / waveNumber;
    float frequency = 2.f / wavelength;
    float speed = angularVelocity / (2.f * 3.142);
    float phaseVelocity = speed * frequency;
    float qi = steepness / (magnitude * frequency * numWaves);
    float rad = frequency * dot(direction.xz, wavePosition.xz) + time * phaseVelocity;
    float sinR = sin(rad);
    float cosR = cos(rad);
    float waFactor = frequency * magnitude;
    
    // derivations from (Mark Finch, 2004)
    // position is as a gertsner wave, with dampening so the edges don't
    // move around
    result.position.x = wavePosition.x + qi * magnitude * direction.x * cosR * edgeDampen;
    result.position.z = wavePosition.z + qi * magnitude * direction.z * cosR * edgeDampen;
    result.position.y = magnitude * sinR * edgeDampen;

    // calculate useful values from new wave position
    float radN = frequency * dot(direction.xyz, result.position) + time * phaseVelocity;
    float sinN = sin(radN);
    float cosN = cos(radN);
    
    // binormal calculation
    result.binormal.x = 1 - (qi * direction.x * direction.x * waFactor * sinN);
    result.binormal.z = -1 * (qi * direction.x * direction.z * waFactor * sinN);
    result.binormal.y = direction.x * waFactor * cosN;
 
    result.tangent.x = -1 * (qi * direction.x * direction.z * waFactor * sinN);
    result.tangent.z = 1 - (qi * direction.z * direction.z * waFactor * sinN);
    result.tangent.y = direction.z * waFactor * cosN;
 
    result.normal.x = -1 * (direction.x * waFactor * cosN);
    result.normal.z = -1 * (direction.z * waFactor * cosN);
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
    
    // (Tardif, n.d.)
    // TODO - parameterise
    float dampeningFactor = 5.f;
    float dampening = 1.0 - pow(saturate(abs(uv.x - 0.5) / 0.5), dampeningFactor);
    dampening *= 1.0 - pow(saturate(abs(uv.y - 0.5) / 0.5), dampeningFactor);
    
    // Create waves
    // (Tardif, n.d.)
    WaveResult finalWaveResult;
    finalWaveResult.position = float3(0.f, 0.f, 0.f);
    finalWaveResult.normal = float3(0.f, 0.f, 0.f);
    finalWaveResult.tangent = float3(0.f, 0.f, 0.f);
    finalWaveResult.binormal = float3(0.f, 0.f, 0.f);
 
    // (Tardif, n.d.)
    for(int waveIndex = 0; waveIndex < numWaves; waveIndex++)
    {
        WaveResult waveResult = CalculateWave(waveData, waveIndex, vertexPosition.xyz, dampening, waveData.totalTime.x);
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
    
    // Output
    output.position = mul(vertexPosition, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);
    float4 worldPosition = mul(vertexPosition, matrixData.worldMatrix);
    // TODO - based off of plane values to scale texture nicely
    // need to parameterise
    output.tex = uv * 10.f;
    output.normal = normal;
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
