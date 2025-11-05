#include "state.h"
#include "core_graphics/texture/texture.h"

#include <filesystem>

void State::initWindowState()
{
    this->windowState.height = 1080;
    this->windowState.width = 1920;
    this->windowState.windowName = "Graphics Project";
}

void State::initD3DAppState(Window *window)
{
    this->d3dAppState.window = window;
}

void State::initShaderManagerState(D3D *graphicsManager)
{
    this->shaderManagerState.graphicsManager = graphicsManager;
    this->shaderManagerState.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    shaderManagerState.pixelShaderDirectory =
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
    this->shaderManagerState.shaders.push_back(
        std::make_unique<VertexShader>(standardVertexShader)
    );

    PixelShader lightingPixelShader = PixelShader();
    lightingPixelShader.filepath = "./build/hlsl/pixel/lighting.cso";
    lightingPixelShader.type = ShaderType::PIXEL;
    this->shaderManagerState.shaders.push_back(
        std::make_unique<PixelShader>(lightingPixelShader)
    );
}

void State::initTextureManagerState(D3D *graphicsManager)
{
    static const uint32_t texturePixelColour = 0x01234567;
    this->textureManagerState.defaultTexture =
        std::make_unique<DefaultTexture>(graphicsManager, texturePixelColour);
    this->textureManagerState.moonTexture = std::make_unique<FileTexture>(
        graphicsManager, "./textures/moon_heightmap.jpg"
    );
    this->textureManagerState.botwHeightMap = std::make_unique<FileTexture>(
        graphicsManager, "./textures/botw_heightmap.png"
    );
    this->textureManagerState.ukHeightMap = std::make_unique<FileTexture>(
        graphicsManager, "./textures/uk_heightmap.jpg"
    );
    this->textureManagerState.mountainsHeightMap =
        std::make_unique<FileTexture>(
            graphicsManager, "./textures/mountains_heightmap.png"
        );
}

void State::initSceneState(
    D3D *graphicsManager, ShaderManager *shaderManager,
    TextureManager *textureManager
)
{
    this->sceneState.graphicsManager = graphicsManager;
    this->sceneState.shaderManager = shaderManager;
    this->sceneState.clearColour = {0.392f, 0.584f, 0.929f, 1.0f};

    // shaders
    this->sceneState.shaderStack.shaders = &shaderManager->state->shaders;

    // sampler
    DefaultSampler defaultSampler = DefaultSampler();

    // meshes
    // TODO - make this a templated function
    // TODO - handle bool returns
    TriangleMesh triangleMesh = TriangleMesh(graphicsManager);
    std::unique_ptr<SceneMesh> triangleSceneMesh =
        std::make_unique<SceneMesh>();
    defaultSampler.init(graphicsManager);
    triangleSceneMesh->mesh = std::make_unique<TriangleMesh>(triangleMesh);
    triangleSceneMesh->worldMatrix = XMMatrixIdentity();
    triangleSceneMesh->shaderStack = &this->sceneState.shaderStack;
    triangleSceneMesh->texture = textureManager->state->defaultTexture.get();
    triangleSceneMesh->sampler =
        std::make_unique<DefaultSampler>(defaultSampler);
    this->sceneState.meshes.push_back(std::move(triangleSceneMesh));

    std::unique_ptr<SceneMesh> sphereSceneMesh = std::make_unique<SceneMesh>();
    SphereMesh sphereMesh = SphereMesh(graphicsManager);
    sphereSceneMesh->mesh = std::make_unique<SphereMesh>(sphereMesh);
    sphereSceneMesh->worldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(-3.f, 0.f, 0.f);
    sphereSceneMesh->shaderStack = &this->sceneState.shaderStack;
    sphereSceneMesh->texture = textureManager->state->moonTexture.get();
    sphereSceneMesh->sampler = std::make_unique<DefaultSampler>(defaultSampler);
    this->sceneState.meshes.push_back(std::move(sphereSceneMesh));

    std::unique_ptr<SceneMesh> quadSceneMesh = std::make_unique<SceneMesh>();
    QuadMesh quadMesh = QuadMesh(graphicsManager);
    quadSceneMesh->mesh = std::make_unique<QuadMesh>(quadMesh);
    quadSceneMesh->worldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(3.f, 0.f, 0.f);
    quadSceneMesh->shaderStack = &this->sceneState.shaderStack;
    quadSceneMesh->texture = textureManager->state->defaultTexture.get();
    quadSceneMesh->sampler = std::make_unique<DefaultSampler>(defaultSampler);
    this->sceneState.meshes.push_back(std::move(quadSceneMesh));

    std::unique_ptr<SceneMesh> planeSceneMesh = std::make_unique<SceneMesh>();
    PlaneMesh planeMesh = PlaneMesh(graphicsManager);
    planeSceneMesh->mesh = std::make_unique<PlaneMesh>(planeMesh);
    planeSceneMesh->worldMatrix =
        XMMatrixIdentity() * XMMatrixTranslation(-10.f, -3.f, -10.f);
    planeSceneMesh->shaderStack = &this->sceneState.shaderStack;
    planeSceneMesh->texture = textureManager->state->botwHeightMap.get();
    planeSceneMesh->sampler = std::make_unique<DefaultSampler>(defaultSampler);
    this->sceneState.meshes.push_back(std::move(planeSceneMesh));

    // lights
    // -- ambient
    std::unique_ptr<AmbientLight> ambientLight =
        std::make_unique<AmbientLight>();
    std::unique_ptr<AmbientLightBuffer> ambientLightBuffer =
        std::make_unique<AmbientLightBuffer>();

    ambientLight->colour = std::make_unique<float[]>(3);
    ambientLight->colour[0] = 0.1f;
    ambientLight->colour[1] = 0.1f;
    ambientLight->colour[2] = 0.1f;
    ambientLight->isEnabled = std::make_unique<bool>(true);

    ambientLightBuffer->init(graphicsManager);

    this->sceneState.ambientLight = std::move(ambientLight);
    ambientLightBuffer->ambientLight = this->sceneState.ambientLight.get();
    this->sceneState.ambientLightBuffer = std::move(ambientLightBuffer);

    // -- point
    std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>();
    std::unique_ptr<PointLightBuffer> pointLightBuffer =
        std::make_unique<PointLightBuffer>();

    pointLight->diffuseColour = std::make_unique<float[]>(3);
    pointLight->diffuseColour[0] = 1.f;
    pointLight->diffuseColour[1] = 1.f;
    pointLight->diffuseColour[2] = 1.f;
    pointLight->specularColour = std::make_unique<float[]>(3);
    pointLight->specularColour[0] = 1.f;
    pointLight->specularColour[1] = 1.f;
    pointLight->specularColour[2] = 1.f;
    pointLight->specularPower = std::make_unique<float>();
    *pointLight->specularPower = 10.f;
    pointLight->worldPosition = std::make_unique<float[]>(3);
    pointLight->worldPosition[0] = 100.f;
    pointLight->worldPosition[1] = 100.f;
    pointLight->worldPosition[2] = 100.f;
    pointLight->attenuationCoefficients = std::make_unique<float[]>(3);
    pointLight->attenuationCoefficients[0] = .432f;
    pointLight->attenuationCoefficients[1] = .00056f;
    pointLight->attenuationCoefficients[2] = .000010f;
    pointLight->isDiffuseEnabled = std::make_unique<bool>(true);
    pointLight->isSpecularEnabled = std::make_unique<bool>(true);
    pointLight->isAttenuationEnabled = std::make_unique<bool>(true);

    pointLightBuffer->init(graphicsManager);
    this->sceneState.pointLight = std::move(pointLight);
    pointLightBuffer->pointLight = this->sceneState.pointLight.get();
    this->sceneState.pointLightBuffer = std::move(pointLightBuffer);

    std::unique_ptr<CameraBuffer> cameraBuffer =
        std::make_unique<CameraBuffer>();
    cameraBuffer->camera = &this->sceneState.camera;
    cameraBuffer->init(graphicsManager);
    this->sceneState.cameraBuffer = std::move(cameraBuffer);
}

void State::initAppInputState(Window *window, Scene *scene)
{
    this->appInputState.window = window;
    this->appInputState.scene = scene;
    this->appInputState.cameraMoveSpeed = 5.f;
    this->appInputState.cameraRotateSensitivity = 0.1f;
}

void State::initAppState(AppInput *appInput)
{
    this->appState.appInput = appInput;
}

void State::initDebugSceneState(
    D3D *graphicsManager, float *fps, float *ambientLightColour,
    float *pointLightDiffuseColour, float *pointLightSpecularColour,
    float *pointLightSpecularPower, float *pointLightWorldPosition,
    float *pointLightAttenuationCoefficients, bool *isAmbientLightEnabled,
    bool *isPointLightDiffuseEnabled, bool *isPointLightSpecularEnabled,
    bool *isPointLightAttenuationEnabled
)
{
    this->debugSceneState.graphicsManager = graphicsManager;
    this->debugSceneState.fps = fps;
    this->debugSceneState.ambientLightColour = ambientLightColour;
    this->debugSceneState.pointLightDiffuseColour = pointLightDiffuseColour;
    this->debugSceneState.pointLightSpecularColour = pointLightSpecularColour;
    this->debugSceneState.pointLightSpecularPower = pointLightSpecularPower;
    this->debugSceneState.pointLightWorldPosition = pointLightWorldPosition;
    this->debugSceneState.pointLightAttenuationCoefficients =
        pointLightAttenuationCoefficients;
    this->debugSceneState.isAmbientLightEnabled = isAmbientLightEnabled;
    this->debugSceneState.isPointLightDiffuseEnabled =
        isPointLightDiffuseEnabled;
    this->debugSceneState.isPointLightSpecularEnabled =
        isPointLightSpecularEnabled;
    this->debugSceneState.isPointLightAttenuationEnabled =
        isPointLightAttenuationEnabled;
}
