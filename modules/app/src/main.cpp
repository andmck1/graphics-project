#include <spdlog/spdlog.h>

#include <core_window/window.h>
#include <core_graphics/d3d/d3d.h>

#include "texture/textureManager.h"
#include "state/state.h"
#include "scene/scene.h"
#include "app/app.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");
    State state = State();

    // Window
    spdlog::info("Creating window");
    state.initWindowState();
    Window window = Window(&state.windowState);

    // D3D app
    state.initD3DAppState(&window);
    spdlog::info("Creating d3d app");
    D3D graphicsManager = D3D(&state.d3dAppState);
    graphicsManager.init();

    // Shader manager
    spdlog::info("Creating shaders");
    state.initShaderManagerState(&graphicsManager);
    ShaderManager shaderManager = ShaderManager(&state.shaderManagerState);

    // Texture manager
    spdlog::info("Creating texture manager");
    state.initTextureManagerState(&graphicsManager);
    TextureManager textureManager = TextureManager(&state.textureManagerState);
    textureManager.loadAll();

    // Scene
    spdlog::info("Creating scene");
    state.initSceneState(&graphicsManager, &shaderManager, &textureManager);
    Scene scene = Scene(&state.sceneState);

    // App
    spdlog::info("Creating app");
    state.initAppInputState(&window, &scene);
    AppInput appInput = AppInput(&state.appInputState);
    state.initAppState(&appInput);
    App app = App(&state.appState);

    // Debug Scene
    spdlog::info("Creating debug scene");
    state.initDebugSceneState(
        &graphicsManager, &app.timer.fps, state.sceneState.ambientLight.get(),
        &state.sceneState.pointLights, &state.sceneState.directionalLights
    );
    DebugScene debugScene = DebugScene(&state.debugSceneState);

    // Main loop
    spdlog::info("Running app");
    app.timer.start();
    while (window.frame())
    {
        app.timer.frame();
        app.state->appInput->handleInputs();
        scene.state->camera.update();
        scene.render();
        debugScene.render();
        graphicsManager.presentScreen();
    }

    return 0;
}
