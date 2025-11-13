#pragma once

#include "base.h"

class BaseQuadMesh : public virtual BaseMesh
{
    public:
    virtual ~BaseQuadMesh() = default;
    virtual void init() override;
};

class QuadMesh : public BaseQuadMesh, public BaseNonTessellatedMesh
{
    public:
    QuadMesh(D3D *graphicsManager);
};

class TessellatedQuadMesh : public BaseQuadMesh, public BaseTessellatedMesh
{
    public:
    TessellatedQuadMesh(D3D *graphicsManager);
};
