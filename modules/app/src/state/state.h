#pragma once

#include "core_window/window.h"
#include "core_graphics/d3d/d3d.h"
#include "core_graphics/shader/shadermanager.h"
#include "../texture/textureManager.h"
#include "../scene/scene.h"
#include "../input/input.h"
#include "../scene/debugScene.h"
#include "../app/app.h"

class State
{
    public:
    WindowState windowState = WindowState();
    D3DAppState d3dAppState = D3DAppState();
    ShaderManagerState shaderManagerState = ShaderManagerState();
    TextureManagerState textureManagerState = TextureManagerState();
    SceneState sceneState = SceneState();
    AppInputState appInputState = AppInputState();
    AppState appState = AppState();
    DebugSceneState debugSceneState = DebugSceneState();

    void initWindowState();
    void initD3DAppState(Window *window);
    void initShaderManagerState(D3D *graphicsManager);
    void initTextureManagerState(D3D *graphicsManager);
    void initSceneState(
        D3D *graphicsManager, ShaderManager *shaderManager,
        TextureManager *textureManager
    );
    void initAppInputState(Window *window, Scene *scene);
    void initAppState(AppInput *appInput);
    void initDebugSceneState(
        D3D *graphicsManager, float *fps, AmbientLight *ambientLight,
        std::vector<std::unique_ptr<PointLight>> *pointLights
    );
};
