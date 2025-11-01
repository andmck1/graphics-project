// https://stackoverflow.com/questions/34886497/how-to-share-a-struct-between-c-directx-and-hlsl
#pragma once

#ifdef __cplusplus
#define SEMANTIC(sem)
#else
#define SEMANTIC(sem) : sem
#endif

#ifdef __cplusplus
#include <directxmath.h>
using namespace DirectX;
using float4 = XMFLOAT4;

namespace LightShader
{
#endif

    struct LightBuffer
    {
        float4 colour;
        float4 worldPosition;
        float4 direction;
    };
#ifdef __cplusplus
}
#endif
