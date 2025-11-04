#pragma once

#include "core_graphics/d3d/d3d.h"
#include "core_graphics/shader/shadermanager.h"
#include "core_window/window.h"
#include "../scene/scene.h"
#include "../scene/debugScene.h"
#include "../input/input.h"
#include "../app/app.h"

class Config
{
    public:
    WindowConfig windowConfig = WindowConfig();
    D3DAppConfig d3dAppConfig = D3DAppConfig();
    ShaderManagerConfig shaderManagerConfig = ShaderManagerConfig();
    SceneConfig sceneConfig = SceneConfig();
    AppInputConfig appInputConfig = AppInputConfig();
    AppConfig appConfig = AppConfig();
    DebugSceneConfig debugSceneConfig = DebugSceneConfig();

    void initWindowConfig();
    void initD3DAppConfig(Window *window);
    void initShaderManagerConfig(D3D *graphicsManager);
    void initSceneConfig(D3D *graphicsManager, ShaderManager *shaderManager);
    void initAppInputConfig(Window *window, Scene *scene);
    void initAppConfig(AppInput *appInput);
    void initDebugSceneConfig(D3D *graphicsManager, float *fps);
};
