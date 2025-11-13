#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/camera/camera.h>

#include "shared_shader/sharedbuffertypes.h"

class CameraBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(Camera *camera);

    struct ShaderBuffer : SharedBufferTypes::CameraBufferType
    {
    };

    Camera *camera;
};


class CameraHullBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(Camera *camera);

    struct ShaderBuffer : SharedBufferTypes::CameraBufferType
    {
    };

    Camera *camera;
};
