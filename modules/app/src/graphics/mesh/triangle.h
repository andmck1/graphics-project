#pragma once

#include "base.h"

class TriangleMesh : public BaseMesh
{
    public:
    TriangleMesh(D3D *graphicsManager);

    void init();
};
