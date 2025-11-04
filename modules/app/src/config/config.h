#pragma once

#include "core_graphics/d3d/d3d.h"
#include "core_graphics/shader/shadermanager.h"
#include "core_window/window.h"
#include "../scene/scene.h"

class Config
{
    public:
    WindowConfig windowConfig = WindowConfig();
    D3DAppConfig d3dAppConfig = D3DAppConfig();
    ShaderManagerConfig shaderManagerConfig = ShaderManagerConfig();
    SceneConfig sceneConfig = SceneConfig();

    bool initWindowConfig();
    bool initD3DAppConfig(Window *window);
    bool initShaderManagerConfig(D3D *graphicsManager);
    bool initSceneConfig(
        D3D *graphicsManager, ShaderManager *shaderManager
    );
};
