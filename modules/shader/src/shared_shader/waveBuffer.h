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

namespace WaveVertexShader
{
#endif
    struct WaveDataBufferType
    {
        float4 totalTime;
        float4 magnitude;
        float4 waveNumber;
        float4 angularVelocity;
    };

#ifdef __cplusplus
}
#endif
