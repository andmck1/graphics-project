#pragma once

#include "core_graphics/actor/base.h"

#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class Camera : public BaseActor
{
    public:
    Camera();
    XMMATRIX viewMatrix;

    void update() override;
};
