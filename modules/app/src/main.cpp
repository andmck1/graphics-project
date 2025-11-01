#include <filesystem>
#include <spdlog/spdlog.h>

#include <core_window/window.h>
#include <core_graphics/d3d/d3d.h>

#include "scene/scene.h"
#include "app/app.h"

int main()
{
    spdlog::set_level(spdlog::level::debug);

    spdlog::info("Starting app");

    // Window
    // TODO - initialise these as pointers?
    // TODO - move this to config file
    WindowSpec windowSpec;
    windowSpec.height = 720;
    windowSpec.width = 1280;
    windowSpec.windowName = "Graphics Project";
    Window window = Window(windowSpec);

    // D3D app
    // TODO - give this proper constructor
    D3DAppConfig d3dAppConfig = D3DAppConfig();
    d3dAppConfig.window = &window;

    D3D graphicsManager = D3D(&d3dAppConfig);
    graphicsManager.init();

    VertexShader standardVertexShader = VertexShader();
    standardVertexShader.filepath = "./build/hlsl/vertex/vertex.cso";
    standardVertexShader.type = ShaderType::VERTEX;
    standardVertexShader.inputElementDescription = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    PixelShader lightingPixelShader = PixelShader();
    lightingPixelShader.filepath = "./build/hlsl/pixel/pixel.cso";
    lightingPixelShader.type = ShaderType::PIXEL;

    // Shader manager
    ShaderManagerConfig shaderManagerConfig = ShaderManagerConfig();
    shaderManagerConfig.graphicsManager = &graphicsManager;
    shaderManagerConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    shaderManagerConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/hlsl/pixel").string();
    shaderManagerConfig.shaders.push_back(&standardVertexShader);
    shaderManagerConfig.shaders.push_back(&lightingPixelShader);
    ShaderManager shaderManager = ShaderManager(&shaderManagerConfig);

    // Scene
    // TODO - give this proper constructor
    SceneConfig sceneConfig = SceneConfig();
    sceneConfig.graphicsManager = &graphicsManager;
    sceneConfig.shaderManager = &shaderManager;
    sceneConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    sceneConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/hlsl/pixel").string();
    Scene scene = Scene(&sceneConfig);

    Camera camera = Camera();
    scene.camera = &camera;

    ShaderStack sceneShaderStack = ShaderStack();

    sceneShaderStack.shaders.push_back(&standardVertexShader);
    sceneShaderStack.shaders.push_back(&lightingPixelShader);

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
