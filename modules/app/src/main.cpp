#include "scene/scene.h"
#include "app/app.h"
#include "window/window.h"
#include "graphics/d3d.h"

#include "spdlog/spdlog.h"
#include <filesystem>

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");

    // TODO - move this to config file
    WindowSpec windowSpec;
    windowSpec.height = 720;
    windowSpec.width = 1280;
    windowSpec.windowName = "Graphics Project";
    Window window = Window(windowSpec);

    // TODO - give this proper constructor
    D3DAppConfig d3dAppConfig = D3DAppConfig();
    d3dAppConfig.window = &window;
    d3dAppConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/shaders/vertex").string();
    d3dAppConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/shaders/pixel").string();
    spdlog::info(d3dAppConfig.vertexShaderDirectory);

    D3D graphicsManager = D3D(&d3dAppConfig);
    graphicsManager.init();

    TriangleMesh triangleMesh = TriangleMesh(&graphicsManager);
    SphereMesh sphereMesh = SphereMesh(&graphicsManager);
    Camera camera = Camera();
    Scene scene = Scene(&graphicsManager);
    scene.triangleMesh = &triangleMesh;
    scene.camera = &camera;
    scene.sphereMesh = &sphereMesh;
    scene.triangleMeshWorldMatrix = XMMatrixIdentity();
    scene.sphereMeshWorldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(-3.f, 0.f, 0.f);

    DebugScene debugScene = DebugScene(&graphicsManager);

    App app = App(&scene, &window);

    const float clearColour[4] = {0.392f, 0.584f, 0.929f, 1.0f};
    while (window.frame())
    {
        app.handleInputs();
        scene.camera->update();
        graphicsManager.clearScreen(clearColour);
        scene.render();
        debugScene.render();
        graphicsManager.presentScreen();
    }

    return 0;
}
