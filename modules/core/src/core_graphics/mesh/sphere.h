#pragma once

#include "base.h"

#include "../d3d/d3d.h"

class SphereMesh : public BaseNonTessellatedMesh
{
    public:
    SphereMesh(D3D *graphicsManager);

    void init() override;
};
