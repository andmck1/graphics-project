#pragma once

#include "base.h"

class PlaneMesh : public BaseNonTessellatedMesh
{
    public:
    PlaneMesh(D3D *graphicsManager);
    void init() override;
};

class TessellatedPlaneMesh : public Base4PointTessellatedMesh
{
    public:
    TessellatedPlaneMesh(D3D *graphicsManager);
    void init() override;
};
