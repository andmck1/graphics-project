#include "config.h"

#include <filesystem>

void Config::initWindowConfig()
{
    this->windowConfig.height = 720;
    this->windowConfig.width = 1280;
    this->windowConfig.windowName = "Graphics Project";
}

void Config::initD3DAppConfig(Window *window)
{
    this->d3dAppConfig.window = window;
}

void Config::initShaderManagerConfig(D3D *graphicsManager)
{
    this->shaderManagerConfig.graphicsManager = graphicsManager;
    this->shaderManagerConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    shaderManagerConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/hlsl/pixel").string();

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
    this->shaderManagerConfig.shaders.push_back(
        std::make_unique<VertexShader>(standardVertexShader)
    );

    PixelShader lightingPixelShader = PixelShader();
    lightingPixelShader.filepath = "./build/hlsl/pixel/pixel.cso";
    lightingPixelShader.type = ShaderType::PIXEL;
    this->shaderManagerConfig.shaders.push_back(
        std::make_unique<PixelShader>(lightingPixelShader)
    );
}

void Config::initSceneConfig(D3D *graphicsManager, ShaderManager *shaderManager)
{
    this->sceneConfig.graphicsManager = graphicsManager;
    this->sceneConfig.shaderManager = shaderManager;
    this->sceneConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    this->sceneConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/hlsl/pixel").string();

    this->sceneConfig.shaderStack.shaders = &shaderManager->config->shaders;

    TriangleMesh triangleMesh = TriangleMesh(graphicsManager);
    SphereMesh sphereMesh = SphereMesh(graphicsManager);
    QuadMesh quadMesh = QuadMesh(graphicsManager);
    PlaneMesh planeMesh = PlaneMesh(graphicsManager);

    std::unique_ptr<SceneMesh> triangleSceneMesh =
        std::make_unique<SceneMesh>();
    std::unique_ptr<SceneMesh> sphereSceneMesh = std::make_unique<SceneMesh>();
    std::unique_ptr<SceneMesh> quadSceneMesh = std::make_unique<SceneMesh>();
    std::unique_ptr<SceneMesh> planeSceneMesh = std::make_unique<SceneMesh>();

    triangleSceneMesh->mesh = std::make_unique<TriangleMesh>(triangleMesh);
    triangleSceneMesh->worldMatrix = XMMatrixIdentity();
    triangleSceneMesh->shaderStack = &this->sceneConfig.shaderStack;
    sphereSceneMesh->mesh = std::make_unique<SphereMesh>(sphereMesh);
    sphereSceneMesh->worldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(-3.f, 0.f, 0.f);
    sphereSceneMesh->shaderStack = &this->sceneConfig.shaderStack;
    quadSceneMesh->mesh = std::make_unique<QuadMesh>(quadMesh);
    quadSceneMesh->worldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(3.f, 0.f, 0.f);
    quadSceneMesh->shaderStack = &this->sceneConfig.shaderStack;
    planeSceneMesh->mesh = std::make_unique<PlaneMesh>(planeMesh);
    planeSceneMesh->worldMatrix = XMMatrixIdentity() * XMMatrixRotationX(-45) *
                                  XMMatrixTranslation(0.f, -3.f, 0.f);
    planeSceneMesh->shaderStack = &this->sceneConfig.shaderStack;

    this->sceneConfig.meshes.push_back(std::move(triangleSceneMesh));
    this->sceneConfig.meshes.push_back(std::move(sphereSceneMesh));
    this->sceneConfig.meshes.push_back(std::move(quadSceneMesh));
    this->sceneConfig.meshes.push_back(std::move(planeSceneMesh));
}

void Config::initAppInputConfig(Window *window, Scene *scene)
{
    this->appInputConfig.window = window;
    this->appInputConfig.scene = scene;
    this->appInputConfig.cameraMoveSpeed = 5.f;
    this->appInputConfig.cameraRotateSensitivity = 0.1f;
}

void Config::initAppConfig(AppInput *appInput)
{
    this->appConfig.appInput = appInput;
}

void Config::initDebugSceneConfig(D3D *graphicsManager, float *fps)
{
    this->debugSceneConfig.graphicsManager = graphicsManager;
    this->debugSceneConfig.fps = fps;
}
