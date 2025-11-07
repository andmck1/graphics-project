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

namespace StandardVertexShader 
{
#endif
    struct CameraBufferType
    {
        float4 worldPosition;
    };
#ifdef __cplusplus
}
#endif
