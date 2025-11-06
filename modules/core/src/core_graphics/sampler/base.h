#pragma once

#include <core_graphics/d3d/d3d.h>

class BaseSampler
{
    public:
    virtual ~BaseSampler() = default;
    virtual bool setPixelShader(D3D *graphicsManager) = 0;
};
