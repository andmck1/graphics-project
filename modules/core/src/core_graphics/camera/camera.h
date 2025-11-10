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
    XMMATRIX orthoViewMatrix;

    void update() override;
};
