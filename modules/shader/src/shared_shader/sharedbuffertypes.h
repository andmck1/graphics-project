#ifdef __cplusplus
#pragma once
#define SEMANTIC(sem)
#else
#define SEMANTIC(sem) : sem
#endif

#ifdef __cplusplus
#include <directxmath.h>
using namespace DirectX;
using float4 = XMFLOAT4;
using int4 = XMINT4;
using matrix = XMMATRIX;
#endif

#ifndef __cplusplus
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};
#endif

#ifdef __cplusplus
namespace SharedBufferTypes
{
#endif

#ifdef __cplusplus
    struct BaseBufferType
    {
    };
#endif

#ifdef __cplusplus
    struct MatrixBufferType : BaseBufferType
#else
struct MatrixBufferType
#endif
    {
        matrix worldMatrix;
        matrix viewMatrix;
        matrix projectionMatrix;
    };

#ifdef __cplusplus
    struct CameraBufferType : BaseBufferType
#else
struct CameraBufferType
#endif
    {
        float4 worldPosition;
    };

#ifdef __cplusplus
    struct AmbientLightBufferType : BaseBufferType
#else
struct AmbientLightBufferType
#endif
    {
        int4 type;
        float4 colour;
        int4 isEnabled;
    };

#ifdef __cplusplus
    struct PointLightBufferType : BaseBufferType
#else
struct PointLightBufferType
#endif
    {
        float4 worldPosition;
        float4 diffuseColour;
        float4 specularColour;
        float4 specularPower;
        float4 attenuationCoefficients;
        int4 isDiffuseEnabled;
        int4 isSpecularEnabled;
        int4 isAttenuationEnabled;
        int4 nLights;
    };

#ifdef __cplusplus
    struct DirectionalLightBufferType : BaseBufferType
#else
struct DirectionalLightBufferType
#endif
    {
        float4 directionVector;
        float4 diffuseColour;
        float4 specularColour;
        float4 specularPower;
        int4 isDiffuseEnabled;
        int4 isSpecularEnabled;
        int4 nLights;
    };

#ifdef __cplusplus
    struct WaveDataBufferType : BaseBufferType
#else
struct WaveDataBufferType
#endif
    {
        float4 totalTime;
        float4 magnitude;
        float4 waveNumber;
        float4 angularVelocity;
    };

#ifdef __cplusplus
}
#endif
