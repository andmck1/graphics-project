#pragma once

#include "core_graphics/actor/scene.h"
#include <directxmath.h>

using namespace DirectX;

class PointLight : public SceneActor
{
    public:
    std::array<float, 3> diffuseColour = {0.f, 0.f, 0.f};
    std::array<float, 3> specularColour = {0.f, 0.f, 0.f};
    float specularPower;
    std::array<float, 3> attenuationCoefficients = {0.f, 0.f, 0.f};
    bool isDiffuseEnabled;
    bool isSpecularEnabled;
    bool isAttenuationEnabled;
};
