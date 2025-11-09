#pragma once

#include "core_graphics/actor/scene.h"
#include <array>
#include <directxmath.h>

using namespace DirectX;

class AmbientLight : public SceneActor
{
    public:
    std::array<float, 3> colour = {0.f, 0.f, 0.f};
    bool isEnabled;
};
