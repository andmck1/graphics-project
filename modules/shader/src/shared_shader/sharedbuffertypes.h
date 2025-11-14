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

// Vertex
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

struct TessellationVertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct TessellationVertexOutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

// Hull and Domain (Tessellation)
struct QuadTessellationHullInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


struct QuadTessellationHullOutputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct QuadTessellationDomainOutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
    float4 spotLightViewPos1 : TEXCOORD3;
    float4 spotLightViewPos2 : TEXCOORD4;
};

struct QuadTessellationConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

// Pixel
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
    float4 depthPosition : TEXCOORD0;
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
    struct MatrixHullBufferType : BaseBufferType
#else
struct MatrixHullBufferType
#endif
    {
        matrix worldMatrix;
        matrix viewMatrix;
        matrix projectionMatrix;
    };

#ifdef __cplusplus
    struct MatrixDomainBufferType : BaseBufferType
#else
struct MatrixDomainBufferType
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
    struct CameraVertexBufferType : BaseBufferType
#else
struct CameraBufferType
#endif
    {
        float4 worldPosition;
    };

#ifdef __cplusplus
    struct CameraHullBufferType : BaseBufferType
#else
struct CameraHullBufferType
#endif
    {
        float4 worldPosition;
    };

#ifdef __cplusplus
    struct CameraDomainBufferType : BaseBufferType
#else
struct CameraDomainBufferType
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
        float4 steepness;
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
    struct TessellationHullDataBufferType : BaseBufferType
#else
struct TessellationHullDataBufferType
#endif
    {
        float4 minTessellationDistanceThreshold;
        float4 maxTessellationDistanceThreshold;
    };

#ifdef __cplusplus
    struct LightSettingsBufferType : BaseBufferType
#else
struct LightSettingsBufferType
#endif
    {
        int4 isNormalsAsColourEnabled;
    };


#ifdef __cplusplus
}
#endif
