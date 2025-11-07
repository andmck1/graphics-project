// https://stackoverflow.com/questions/34886497/how-to-share-a-struct-between-c-directx-and-hlsl

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

namespace LightingShader
{
#endif
    struct AmbientLightBufferType
    {
        int4 type;
        float4 colour;
        int4 isEnabled;
    };

    struct PointLightBufferType
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

    struct DirectionalLightBufferType
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
}
#endif
