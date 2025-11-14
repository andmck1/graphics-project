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

float getWaveHeight(float4 position, WaveDataBufferType _waveData)
{
    float heightDisplacement;
    heightDisplacement = (
		(
			_waveData.magnitude.x * 
			sin(
				(_waveData.waveNumber.x * position.x) - 
				(_waveData.angularVelocity.x * _waveData.totalTime.x)
			)
		) + 
		(
			_waveData.magnitude.x * 
			cos(
				(_waveData.waveNumber.x * position.z) - 
				(_waveData.angularVelocity.x * _waveData.totalTime.x)
				)
		)
	);
    return heightDisplacement;
}

float3 getWaveNormal(float4 position, WaveDataBufferType _waveData)
{	
    float3 normal = float3(0.f, 0.f, 0.f);
    normal.x = (
		-_waveData.magnitude.x * 
		_waveData.waveNumber.x * 
		cos(
			(_waveData.waveNumber.x * position.x)  - 
			(_waveData.angularVelocity.x * _waveData.totalTime.x)
		)
	);
    normal.y = 1.0f;
    normal.z = (
		_waveData.magnitude.x * 
		_waveData.waveNumber.x * 
		sin(
			(_waveData.waveNumber.x * position.z) - 
			(_waveData.angularVelocity.x * _waveData.totalTime.x)
		)
	);
    return normalize(normal);
}

[domain("quad")]
QuadTessellationDomainOutputType main(
    QuadTessellationConstantOutputType input, 
    float2 uvPatch: SV_DomainLocation, 
    const OutputPatch<QuadTessellationHullOutputType, 4> patch
)
{
    QuadTessellationDomainOutputType output;
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
    waveDisplacement = getWaveHeight(vertexPosition4f, waveData);
    waveNormal = getWaveNormal(vertexPosition4f, waveData);
    vertexPosition4f.y += waveDisplacement;
    normal = waveNormal;
    // vertexPosition4f.xyz += normal.xyz;	

    output.position = mul(vertexPosition4f, matrixData.worldMatrix);
    output.position = mul(output.position, matrixData.viewMatrix);
    output.position = mul(output.position, matrixData.projectionMatrix);

    output.tex = uv;

    output.normal = normal;

    float4 worldPosition = mul(vertexPosition4f, matrixData.worldMatrix);
    output.worldPosition = worldPosition.xyz;
    output.viewVector = normalize(cameraData.worldPosition.xyz - worldPosition.xyz);

    output.spotLightViewPos1 = mul(vertexPosition4f, matrixData.worldMatrix);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightViewMatrix1);
    output.spotLightViewPos1 = mul(output.spotLightViewPos1, matrixData.spotLightProjectionMatrix1);
    output.spotLightViewPos2 = mul(vertexPosition4f, matrixData.worldMatrix);
    output.spotLightViewPos2 = mul(output.spotLightViewPos2, matrixData.spotLightViewMatrix2);
    output.spotLightViewPos2 = mul(output.spotLightViewPos2, matrixData.spotLightProjectionMatrix2);
 
    return output;
}