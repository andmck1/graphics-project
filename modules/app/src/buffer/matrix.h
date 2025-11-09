#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/camera/camera.h>

#include "shared_shader/sharedbuffertypes.h"

class MatrixBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix
    );

    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;

    struct ShaderBuffer : SharedBufferTypes::MatrixBufferType
    {
    };
};
