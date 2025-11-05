#pragma once

#include <memory>
#include <directxmath.h>

using namespace DirectX;

struct PointLight 
{
    std::unique_ptr<float[]> diffuseColour;
    std::unique_ptr<float[]> specularColour;
    std::unique_ptr<float> specularPower;
    std::unique_ptr<float[]> worldPosition;
    std::unique_ptr<float[]> attenuationCoefficients;
    std::unique_ptr<bool> isDiffuseEnabled;
    std::unique_ptr<bool> isSpecularEnabled;
    std::unique_ptr<bool> isAttenuationEnabled;
};
