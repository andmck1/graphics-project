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

// TODO - repeated structs here
#ifndef __cplusplus
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VertexOutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
    float4 spotLightViewPos1 : TEXCOORD3;
    float4 spotLightViewPos2 : TEXCOORD4;
};

struct DepthVertexOutputType
{
    float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
    float4 spotLightViewPos1 : TEXCOORD3;
    float4 spotLightViewPos2 : TEXCOORD4;
};

struct DepthPixelInputType
{
    float4 position : SV_POSITION;
    float4 depthPosition: TEXCOORD0;
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
        matrix spotLightViewMatrix1;
        matrix spotLightProjectionMatrix1;
        matrix spotLightViewMatrix2;
        matrix spotLightProjectionMatrix2;
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
    struct SpotLightBufferType : BaseBufferType
#else
struct SpotLightBufferType
#endif
    {
        float4 worldPosition;
        float4 directionVector;
        float4 coneAngle;
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
    struct CRTBufferType : BaseBufferType
#else
struct CRTBufferType
#endif
    {
        float4 totalTime;

        int4 isVHSWiggleEnabled;
        int4 isCurvingEnabled;
        int4 isChromaticAberrationEnabled;
        int4 isGrainEnabled;
        int4 isRGBGridEnabled;
        int4 isScanlinesEnabled;
        int4 isVingetteEnabled;

        float4 brightnessMultiplier;
        float4 grainStrength;
        float4 curvePower;
        float4 vingetteStrength;
        int4 scanlinesInterval;
        float4 scanlinesOpacity;
        int4 scanlinesThickness;
        float4 chromaticAberrationStrength;
        float4 vhsWiggle;
    };

#ifdef __cplusplus
}
#endif
