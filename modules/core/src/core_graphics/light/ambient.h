#pragma once

#include <memory>
#include <directxmath.h>

using namespace DirectX;

struct AmbientLight
{
    std::unique_ptr<float[]> colour;
    std::unique_ptr<bool> isEnabled;
};
