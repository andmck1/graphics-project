#pragma once

#include <DirectXMath.h>
#include <vector>
#include "graphics/meshes/base.h"

struct Scene
{
    template <typename T> Scene(T t);
    std::vector<BaseMesh> meshes;
    DirectX::XMMATRIX worldMatrix;
};
