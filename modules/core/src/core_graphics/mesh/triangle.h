#pragma once

#include "base.h"

class BaseTriangleMesh : public virtual BaseMesh
{
    public:
    virtual ~BaseTriangleMesh() = default;
    virtual void init() override;
};

class TriangleMesh : public BaseTriangleMesh, public BaseNonTessellatedMesh
{
    public:
    TriangleMesh(D3D *graphicsManager);
};

class TessellatedTriangleMesh : public BaseTriangleMesh,
                                public Base3PointTessellatedMesh
{
    public:
    TessellatedTriangleMesh(D3D *graphicsManager);
};
