#pragma once

#include "base.h"

#include "../d3d/d3d.h"

class SphereMesh : public BaseMesh
{
    public:
    SphereMesh(D3D *graphicsManager);

    void init();
};
