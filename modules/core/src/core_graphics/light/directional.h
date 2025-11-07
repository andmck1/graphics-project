#pragma once

#include <core_graphics/scene/base.h>
#include <memory>
#include <directxmath.h>

using namespace DirectX;

struct DirectionalLight
{
    std::unique_ptr<float[]> diffuseColour;
    std::unique_ptr<float[]> specularColour;
    std::unique_ptr<float> specularPower;
    std::unique_ptr<float[]> directionVector;
    std::unique_ptr<bool> isDiffuseEnabled;
    std::unique_ptr<bool> isSpecularEnabled;
};
