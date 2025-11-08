#pragma once

#include "core_graphics/actor/scene.h"
#include <array>
#include <directxmath.h>

using namespace DirectX;

class AmbientLight : SceneActor
{
    public:
    std::array<float, 3> colour;
    bool isEnabled;

    bool isDrawEnabled = false;
};
