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
    static const int LIGHT_TYPE_AMBIENT = 0;
    static const int LIGHT_TYPE_POINT = 1;
    static const int LIGHT_TYPE_DIRECTIONAL = 2;
    static const int LIGHT_TYPE_SPOT = 3;

    struct AmbientLightBuffer
    {
        int4 type;
        float4 colour;
        int4 isEnabled;
    };

    struct PointLightBuffer
    {
        int4 type;
        float4 worldPosition;
        float4 diffuseColour;
        float4 specularColour;
        float4 specularPower;
        float4 attenuationCoefficients;
        int4 isDiffuseEnabled;
        int4 isSpecularEnabled;
        int4 isAttenuationEnabled;
    };

    struct DirectionalLightBuffer
    {
        int4 type;
        float4 worldPosition;
        float4 diffuseColour;
        float4 specularColour;
        float4 attenuationCoefficients;
        float4 directionVector;
    };

#ifdef __cplusplus
}
#endif
