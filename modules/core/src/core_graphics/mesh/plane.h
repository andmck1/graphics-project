#pragma once

#include "base.h"

class PlaneMesh : public BaseNonTessellatedMesh
{
    public:
    PlaneMesh(D3D *graphicsManager);

    void init() override;
};
