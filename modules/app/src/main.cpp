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
    config.initWindowConfig();
    Window window = Window(&config.windowConfig);

    // D3D app
    config.initD3DAppConfig(&window);
    D3D graphicsManager = D3D(&config.d3dAppConfig);
    graphicsManager.init();

    // Shader manager
    config.initShaderManagerConfig(&graphicsManager);
    ShaderManager shaderManager = ShaderManager(&config.shaderManagerConfig);

    // Scene
    config.initSceneConfig(&graphicsManager, &shaderManager);
    Scene scene = Scene(&config.sceneConfig);

    Camera camera = Camera();
    scene.camera = &camera;

    ShaderStack sceneShaderStack = ShaderStack();
    sceneShaderStack.shaders = &config.shaderManagerConfig.shaders;

    TriangleMesh triangleMesh = TriangleMesh(&graphicsManager);
    SphereMesh sphereMesh = SphereMesh(&graphicsManager);
    QuadMesh quadMesh = QuadMesh(&graphicsManager);
    PlaneMesh planeMesh = PlaneMesh(&graphicsManager);

    SceneMesh triangleSceneMesh = SceneMesh();
    SceneMesh sphereSceneMesh = SceneMesh();
    SceneMesh quadSceneMesh = SceneMesh();
    SceneMesh planeSceneMesh = SceneMesh();

    triangleSceneMesh.mesh = &triangleMesh;
    triangleSceneMesh.worldMatrix = XMMatrixIdentity();
    triangleSceneMesh.shaderStack = &sceneShaderStack;
    sphereSceneMesh.mesh = &sphereMesh;
    sphereSceneMesh.worldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(-3.f, 0.f, 0.f);
    sphereSceneMesh.shaderStack = &sceneShaderStack;
    quadSceneMesh.mesh = &quadMesh;
    quadSceneMesh.worldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(3.f, 0.f, 0.f);
    quadSceneMesh.shaderStack = &sceneShaderStack;
    planeSceneMesh.mesh = &planeMesh;
    planeSceneMesh.worldMatrix = XMMatrixIdentity() * XMMatrixRotationX(-45) *
                                 XMMatrixTranslation(0.f, -3.f, 0.f);
    planeSceneMesh.shaderStack = &sceneShaderStack;

    scene.meshes.push_back(triangleSceneMesh);
    scene.meshes.push_back(sphereSceneMesh);
    scene.meshes.push_back(quadSceneMesh);
    scene.meshes.push_back(planeSceneMesh);

    // App
    App app = App(&scene, &window);

    // Debug Scene
    DebugSceneConfig debugSceneConfig = DebugSceneConfig();
    debugSceneConfig.graphicsManager = &graphicsManager;
    debugSceneConfig.fps = &app.timer.fps;
    DebugScene debugScene = DebugScene(&debugSceneConfig);

    // Main loop
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
