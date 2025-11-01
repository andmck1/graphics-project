#include <filesystem>
#include <spdlog/spdlog.h>

#include <core_window/window.h>
#include <core_graphics/d3d/d3d.h>

#include "core_graphics/mesh/ortho.h"
#include "core_graphics/mesh/quad.h"
#include "scene/scene.h"
#include "app/app.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");

    // TODO - initialise these as pointers?
    // TODO - move this to config file
    WindowSpec windowSpec;
    windowSpec.height = 720;
    windowSpec.width = 1280;
    windowSpec.windowName = "Graphics Project";
    Window window = Window(windowSpec);

    // TODO - give this proper constructor
    D3DAppConfig d3dAppConfig = D3DAppConfig();
    d3dAppConfig.window = &window;

    D3D graphicsManager = D3D(&d3dAppConfig);
    graphicsManager.init();

    // TODO - give this proper constructor
    SceneConfig sceneConfig = SceneConfig();
    ShaderManager shaderManager = ShaderManager(&graphicsManager);
    sceneConfig.graphicsManager = &graphicsManager;
    sceneConfig.shaderManager = &shaderManager;
    sceneConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    sceneConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/hlsl/pixel").string();

    TriangleMesh triangleMesh = TriangleMesh(&graphicsManager);
    SphereMesh sphereMesh = SphereMesh(&graphicsManager);
    QuadMesh quadMesh = QuadMesh(&graphicsManager);
    PlaneMesh planeMesh = PlaneMesh(&graphicsManager);
    OrthoMesh orthoMesh =
        OrthoMesh(&graphicsManager, windowSpec.height, windowSpec.width, 0, 0);

    Camera camera = Camera();
    Scene scene = Scene(&sceneConfig);

    scene.camera = &camera;
    scene.triangleMesh = &triangleMesh;
    scene.sphereMesh = &sphereMesh;
    scene.quadMesh = &quadMesh;
    scene.planeMesh = &planeMesh;
    scene.orthoMesh = &orthoMesh;
    scene.triangleMeshWorldMatrix = XMMatrixIdentity();
    scene.sphereMeshWorldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(-3.f, 0.f, 0.f);
    scene.quadMeshWorldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(3.f, 0.f, 0.f);
    scene.planeMeshWorldMatrix = XMMatrixIdentity() * XMMatrixRotationX(-45) *
                                 XMMatrixTranslation(0.f, -3.f, 0.f);
    scene.orthoMeshWorldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(0.f, 3.f, 0.f);

    App app = App(&scene, &window);

    DebugSceneConfig debugSceneConfig = DebugSceneConfig();
    debugSceneConfig.graphicsManager = &graphicsManager;
    debugSceneConfig.fps = &app.timer.fps;
    DebugScene debugScene = DebugScene(&debugSceneConfig);

    const float clearColour[4] = {0.392f, 0.584f, 0.929f, 1.0f};
    app.timer.start();
    while (window.frame())
    {
        app.timer.frame();
        app.handleInputs();
        scene.camera->update();
        graphicsManager.clearScreen(clearColour);
        scene.render();
        debugScene.render();
        graphicsManager.presentScreen();
    }

    return 0;
}
