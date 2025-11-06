#pragma once

#include "core_graphics/light/ambient.h"
#include "core_graphics/light/point.h"
#include <core_graphics/d3d/d3d.h>
#include <memory>
#include <vector>

struct DebugSceneState
{
    D3D *graphicsManager;
    float *fps;

    AmbientLight* ambientLight;
    std::vector<std::unique_ptr<PointLight>> *pointLights;
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
