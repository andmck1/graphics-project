#pragma once

#include <core_graphics/d3d/d3d.h>

struct DebugSceneState
{
    D3D *graphicsManager;
    float *fps;

    bool *isAmbientLightEnabled;
    float *ambientLightColour;

    float *pointLightDiffuseColour;
    float *pointLightSpecularColour;
    float *pointLightSpecularPower;
    float *pointLightWorldPosition;
    float *pointLightAttenuationCoefficients;
    bool *isPointLightDiffuseEnabled;
    bool *isPointLightSpecularEnabled;
    bool *isPointLightAttenuationEnabled;
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
