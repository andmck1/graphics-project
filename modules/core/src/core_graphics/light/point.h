#pragma once

#include "core_graphics/actor/scene.h"
#include <directxmath.h>

using namespace DirectX;

class PointLight : public SceneActor
{
    public:
    std::array<float, 3> diffuseColour;
    std::array<float, 3> specularColour;
    float specularPower;
    std::array<float, 3> attenuationCoefficients;
    bool isDiffuseEnabled;
    bool isSpecularEnabled;
    bool isAttenuationEnabled;

};
