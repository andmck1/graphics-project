#pragma once

#include <core_graphics/d3d/d3d.h>

struct DebugSceneConfig
{
    D3D *graphicsManager;
    float *fps;
};

class DebugScene
{
    public:
    DebugScene(DebugSceneConfig *config);
    ~DebugScene();
    bool render();

    private:
    DebugSceneConfig *config;
};
