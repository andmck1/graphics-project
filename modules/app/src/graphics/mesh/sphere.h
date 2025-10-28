#pragma once

#include "../../graphics/d3d.h"
#include "base.h"
#include <d3d11.h>
#include <directxmath.h>

class SphereMesh : public BaseMesh
{
    public:
    SphereMesh(D3D *graphicsManager);

    void init();
};
