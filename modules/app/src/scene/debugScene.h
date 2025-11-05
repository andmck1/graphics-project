#pragma once

#include <core_graphics/d3d/d3d.h>

struct DebugSceneState
{
    D3D *graphicsManager;
    float *fps;
    float *ambientLightColour;
};

class DebugScene
{
    public:
    DebugScene(DebugSceneState *state);
    ~DebugScene();
    bool render();

    private:
    DebugSceneState *state;
};
