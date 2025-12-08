#include <cstdlib>
#include <directxmath.h>
#include <spdlog/spdlog.h>

#include <core_window/window.h>
#include <core_graphics/d3d/d3d.h>

#include "texturemanager/texturemanager.h"
#include "state/state.h"
#include "scene/scene.h"
#include "app/app.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");
    State state = State();

    // App
    spdlog::info("Creating app");
    state.initAppState();
    App app = App(&state.appState);

    // Window
    spdlog::info("Creating window");
    state.initWindowState();
    Window window = Window(&state.windowState);

    // D3D app
    state.initD3DAppState(&window);
    spdlog::info("Creating d3d app");
    D3D graphicsManager = D3D(&state.d3dAppState);
    graphicsManager.init();

    // Shaders
    spdlog::info("Creating shaders");
    state.initShaderManagerState(&graphicsManager);

    // Texture manager
    state.initTextureManagerState(&graphicsManager);
    TextureManager textureManager = TextureManager(&state.textureManagerState);
    spdlog::info("Creating texture manager");
    textureManager.loadAll();

    // Scene
    spdlog::info("Creating scene");
    state.initSceneState(&graphicsManager, &state.shaderManagerState);
    Scene scene = Scene(&state.sceneState);
    state.setShaderManagerStateShaderBufferData();

    // App Input
    spdlog::info("Creating app input");
    state.initAppInputState(&window, &scene, &app.timer);
    AppInput appInput = AppInput(&state.appInputState);

    // Debug Scene
    spdlog::info("Creating debug scene");
    state.initDebugSceneState(
        &graphicsManager, &app.timer.fps, &scene.state->isWireframeEnabled,
        &scene.state->isNormalsAsColourEnabled, &scene.state->shadowBias,
        state.sceneState.ambientLight, &state.sceneState.pointLights,
        &state.sceneState.spotLights, &state.sceneState.directionalLights,
        state.sceneState.water, &state.sceneState.crtSettings,
        &state.sceneState.crtPostProcessor.isEnabled,
        &state.sceneState.tessellationHullDataSettings, state.sceneState.camera
    );
    DebugScene debugScene = DebugScene(&state.debugSceneState);

    // Main loop
    spdlog::info("Running app");
    app.timer.start();
    while (window.frame())
    {
        app.timer.frame();
        appInput.handleInputs();

        for (SpotLight *spotlight : scene.state->spotLights)
        {
            XMMATRIX spotlightRotation =
                XMMatrixTranslation(-1500.f, 0.f, -1500.f) *
                XMMatrixRotationY(0.001f) *
                XMMatrixTranslation(1500.f, 0.f, 1500.f);
            XMVECTOR newWorldPosition = XMVector3Transform(
                XMVectorSet(
                    spotlight->worldPosition[0], spotlight->worldPosition[1],
                    spotlight->worldPosition[2], 1.f
                ),
                spotlightRotation
            );
            spotlight->worldPosition[0] = newWorldPosition[0];
            spotlight->worldPosition[1] = newWorldPosition[1];
            spotlight->worldPosition[2] = newWorldPosition[2];
            XMVECTOR newDirectionVector = XMVector3Normalize(
                -newWorldPosition + XMVectorSet(1250.f, 0.f, 1250.f, 0.f)
            );
            spotlight->directionVector[0] = newDirectionVector[0];
            spotlight->directionVector[1] = newDirectionVector[1];
            spotlight->directionVector[2] = newDirectionVector[2];
        }

        scene.updateAndDraw();
        debugScene.render();
        graphicsManager.presentScreen();
    }

    return 0;
}
