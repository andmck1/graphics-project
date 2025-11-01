#pragma once

#include "base.h"

class OrthoMesh : public BaseMesh
{
    public:
    OrthoMesh(
        D3D *graphicsManager, int height, int width, int xPosition,
        int yPosition
    );

    int height;
    int width;
    int xPosition;
    int yPosition;

    void init();
};
