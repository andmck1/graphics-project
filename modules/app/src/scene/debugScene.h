#pragma once

#include "core_graphics/light/ambient.h"
#include "core_graphics/light/point.h"
#include "core_graphics/light/directional.h"
#include "core_graphics/light/spotlight.h"
#include "core_graphics/water/water.h"
#include "shared_shader/sharedbuffertypes.h"
#include <core_graphics/d3d/d3d.h>
#include <vector>

struct DebugSceneState
{
    D3D *graphicsManager;
    float *fps;
    bool *isWireframeEnabled;

    AmbientLight *ambientLight;
    std::vector<PointLight *> *pointLights;
    std::vector<DirectionalLight *> *directionalLights;
    std::vector<SpotLight *> *spotLights;
    Water *water;
    SharedBufferTypes::CRTBufferType *crtSettings;
    SharedBufferTypes::TessellationHullDataBufferType *tessellationSettings;
    bool *isCRTEnabled;
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
