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
        &scene.state->isNormalsAsColourEnabled, state.sceneState.ambientLight,
        &state.sceneState.pointLights, &state.sceneState.spotLights,
        &state.sceneState.directionalLights, state.sceneState.water,
        &state.sceneState.crtSettings,
        &state.sceneState.crtPostProcessor.isEnabled,
        &state.sceneState.tessellationHullDataSettings
    );
    DebugScene debugScene = DebugScene(&state.debugSceneState);

    // Main loop
    spdlog::info("Running app");
    app.timer.start();
    float rotationIncrement = 0.1;
    while (window.frame())
    {
        app.timer.frame();
        appInput.handleInputs();
        scene.state->spotLights.back()->directionVector[0] +=
            rotationIncrement * app.timer.deltaTime;
        scene.state->spotLights.back()->directionVector[1] +=
            rotationIncrement * app.timer.deltaTime;
        if (std::abs(scene.state->spotLights.back()->directionVector[0]) > 0.25f)
            rotationIncrement *= -1.f;
        if (std::abs(scene.state->spotLights.back()->directionVector[2]) > 0.25f)
            rotationIncrement *= -1.f;
        scene.updateAndDraw();
        debugScene.render();
        graphicsManager.presentScreen();
    }

    return 0;
}
