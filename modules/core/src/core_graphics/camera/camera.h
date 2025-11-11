#pragma once

#include "core_graphics/actor/scene.h"

#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class Camera : public SceneActor
{
    public:
    Camera();
    XMMATRIX viewMatrix;
    XMMATRIX orthographicViewMatrix;
    XMFLOAT2 movementDirectionVector = {0.f, 0.f};

    void update() override;
};
