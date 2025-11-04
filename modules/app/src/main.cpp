#include <filesystem>
#include <spdlog/spdlog.h>

#include <core_window/window.h>
#include <core_graphics/d3d/d3d.h>

#include "config/config.h"
#include "scene/scene.h"
#include "app/app.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");
    Config config = Config();

    // Window
    spdlog::info("Creating window");
    config.initWindowConfig();
    Window window = Window(&config.windowConfig);

    // D3D app
    config.initD3DAppConfig(&window);
    spdlog::info("Creating d3d app");
    D3D graphicsManager = D3D(&config.d3dAppConfig);
    graphicsManager.init();

    // Shader manager
    spdlog::info("Creating shaders");
    config.initShaderManagerConfig(&graphicsManager);
    ShaderManager shaderManager = ShaderManager(&config.shaderManagerConfig);

    // Scene
    spdlog::info("Creating scene");
    config.initSceneConfig(&graphicsManager, &shaderManager);
    Scene scene = Scene(&config.sceneConfig);

    // App
    spdlog::info("Creating app");
    config.initAppInputConfig(&window, &scene);
    AppInput appInput = AppInput(&config.appInputConfig);
    config.initAppConfig(&appInput);
    App app = App(&config.appConfig);

    // Debug Scene
    spdlog::info("Creating debug scene");
    config.initDebugSceneConfig(&graphicsManager, &app.timer.fps);
    DebugScene debugScene = DebugScene(&config.debugSceneConfig);

    // Main loop
    const float clearColour[4] = {0.392f, 0.584f, 0.929f, 1.0f};
    app.timer.start();
    while (window.frame())
    {
        app.timer.frame();
        app.config->appInput->handleInputs(app.timer.deltaTime);
        scene.camera->update();
        graphicsManager.clearScreen(clearColour);
        scene.render();
        debugScene.render();
        graphicsManager.presentScreen();
    }

    return 0;
}
