#pragma once

#include <memory>
#include <directxmath.h>

using namespace DirectX;

struct AmbientLight
{
    std::unique_ptr<float[]> colour;
};
