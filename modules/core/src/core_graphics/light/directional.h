#pragma once

#include "core_graphics/actor/scene.h"
#include <array>
#include <directxmath.h>

using namespace DirectX;

class DirectionalLight : public SceneActor
{
    public:
    std::array<float, 3> diffuseColour;
    std::array<float, 3> specularColour;
    float specularPower;
    std::array<float, 3> directionVector;
    bool isDiffuseEnabled;
    bool isSpecularEnabled;

    bool isDrawEnabled = false;
};
