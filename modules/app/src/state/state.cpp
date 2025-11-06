#include "state.h"
#include "core_graphics/light/ambient.h"
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
    BaseShader *standardVertexShaderPtr =
        this->shaderManagerState.shaders.back().get();

    PixelShader lightingPixelShader = PixelShader();
    lightingPixelShader.filepath = "./build/hlsl/pixel/lighting.cso";
    lightingPixelShader.type = ShaderType::PIXEL;
    this->shaderManagerState.shaders.push_back(
        std::make_unique<PixelShader>(lightingPixelShader)
    );
    BaseShader *lightingPixelShaderPtr =
        this->shaderManagerState.shaders.back().get();

    PixelShader lightSourcePixelShader = PixelShader();
    lightSourcePixelShader.filepath = "./build/hlsl/pixel/lightSource.cso";
    lightSourcePixelShader.type = ShaderType::PIXEL;
    this->shaderManagerState.shaders.push_back(
        std::make_unique<PixelShader>(lightSourcePixelShader)
    );
    BaseShader *lightSourcePixelShaderPtr =
        this->shaderManagerState.shaders.back().get();

    this->shaderManagerState.defaultShaderStack.shaders =
        std::vector({standardVertexShaderPtr, lightingPixelShaderPtr});
    this->shaderManagerState.lightSourceShaderStack.shaders =
        std::vector({standardVertexShaderPtr, lightSourcePixelShaderPtr});
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
    this->textureManagerState.lightDebugTexture = std::make_unique<FileTexture>(
        graphicsManager, "./textures/light_debug.jpg"
    );
}

void State::initSceneStateSampler()
{
    DefaultSampler _defaultSampler = DefaultSampler();
    _defaultSampler.init(this->sceneState.graphicsManager);
    this->sceneState.defaultSampler =
        std::move(std::make_unique<DefaultSampler>(_defaultSampler));
}

template <typename MeshType>
void State::initSceneStateMesh(
    ShaderStack *shaderStack, BaseTexture *texture, BaseSampler *sampler,
    XMMATRIX initScaling, XMMATRIX initRotation, XMMATRIX initTranslation
)
{
    // TODO - handle bool returns
    MeshType mesh = MeshType(this->sceneState.graphicsManager);
    std::unique_ptr<SceneMesh> sceneMesh = std::make_unique<SceneMesh>();
    sceneMesh->mesh = std::make_unique<MeshType>(mesh);
    sceneMesh->worldMatrix =
        XMMatrixIdentity() * initScaling * initRotation * initTranslation;
    sceneMesh->shaderStack = shaderStack;
    sceneMesh->texture = texture;
    sceneMesh->sampler = sampler;
    this->sceneState.meshes.push_back(std::move(sceneMesh));
}

void State::initSceneStateMeshes()
{
    this->initSceneStateMesh<TriangleMesh>(
        &this->shaderManagerState.defaultShaderStack,
        this->sceneState.textureManager->state->defaultTexture.get(),
        this->sceneState.defaultSampler.get()
    );

    this->initSceneStateMesh<SphereMesh>(
        &this->shaderManagerState.defaultShaderStack,
        this->sceneState.textureManager->state->moonTexture.get(),
        this->sceneState.defaultSampler.get(), XMMatrixIdentity(),
        XMMatrixIdentity(), XMMatrixTranslation(-3.f, 0.f, 0.f)
    );

    this->initSceneStateMesh<QuadMesh>(
        &this->shaderManagerState.defaultShaderStack,
        this->sceneState.textureManager->state->defaultTexture.get(),
        this->sceneState.defaultSampler.get(), XMMatrixIdentity(),
        XMMatrixIdentity(), XMMatrixTranslation(3.f, 0.f, 0.f)
    );

    this->initSceneStateMesh<PlaneMesh>(
        &this->shaderManagerState.defaultShaderStack,
        this->sceneState.textureManager->state->botwHeightMap.get(),
        this->sceneState.defaultSampler.get(), XMMatrixIdentity(),
        XMMatrixIdentity(), XMMatrixTranslation(-10.f, -3.f, -10.f)
    );
}

void State::initSceneStateAmbientLight()
{
    std::unique_ptr<AmbientLight> ambientLight =
        std::make_unique<AmbientLight>();
    std::unique_ptr<AmbientLightBuffer> ambientLightBuffer =
        std::make_unique<AmbientLightBuffer>();

    ambientLight->colour = std::make_unique<float[]>(3);
    ambientLight->colour[0] = 0.1f;
    ambientLight->colour[1] = 0.1f;
    ambientLight->colour[2] = 0.1f;
    ambientLight->isEnabled = std::make_unique<bool>(true);

    ambientLightBuffer->init(this->sceneState.graphicsManager);

    this->sceneState.ambientLight = std::move(ambientLight);
    ambientLightBuffer->ambientLight = this->sceneState.ambientLight.get();
    this->sceneState.ambientLightBuffer = std::move(ambientLightBuffer);
}

void State::initSceneStatePointLight(
    XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
    float initSpecularPower, XMFLOAT3 initWorldPosition,
    XMFLOAT3 initAttenuationCoefficients, bool initIsDiffuseEnabled,
    bool initIsSpecularEnabled, bool initIsAttenuationEnabled
)
{
    std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>();
    std::unique_ptr<PointLightsBuffer> pointLightsBuffer =
        std::make_unique<PointLightsBuffer>();

    this->initSceneStateMesh<SphereMesh>(
        &this->shaderManagerState.lightSourceShaderStack,
        this->sceneState.textureManager->state->lightDebugTexture.get(),
        this->sceneState.defaultSampler.get(), XMMatrixScaling(.1f, .1f, .1f),
        XMMatrixIdentity(),
        XMMatrixTranslation(
            initWorldPosition.x, initWorldPosition.y, initWorldPosition.z
        )
    );

    pointLight->diffuseColour = std::make_unique<float[]>(3);
    pointLight->diffuseColour[0] = initDiffuseColour.x;
    pointLight->diffuseColour[1] = initDiffuseColour.y;
    pointLight->diffuseColour[2] = initDiffuseColour.z;
    pointLight->specularColour = std::make_unique<float[]>(3);
    pointLight->specularColour[0] = initSpecularColour.x;
    pointLight->specularColour[1] = initSpecularColour.y;
    pointLight->specularColour[2] = initSpecularColour.z;
    pointLight->specularPower = std::make_unique<float>();
    *pointLight->specularPower = initSpecularPower;
    pointLight->worldPosition = std::make_unique<float[]>(3);
    pointLight->worldPosition[0] = initWorldPosition.x;
    pointLight->worldPosition[1] = initWorldPosition.y;
    pointLight->worldPosition[2] = initWorldPosition.z;
    pointLight->attenuationCoefficients = std::make_unique<float[]>(3);
    pointLight->attenuationCoefficients[0] = initAttenuationCoefficients.x;
    pointLight->attenuationCoefficients[1] = initAttenuationCoefficients.y;
    pointLight->attenuationCoefficients[2] = initAttenuationCoefficients.z;
    pointLight->isDiffuseEnabled = std::make_unique<bool>(initIsDiffuseEnabled);
    pointLight->isSpecularEnabled =
        std::make_unique<bool>(initIsSpecularEnabled);
    pointLight->isAttenuationEnabled =
        std::make_unique<bool>(initIsAttenuationEnabled);
    pointLight->sceneMesh = this->sceneState.meshes.back().get();

    this->sceneState.pointLights.push_back(std::move(pointLight));
}

void State::initSceneStatePointLights()
{
    this->initSceneStatePointLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(.432f, .00056f, .000010f), true, true,
        true
    );
    this->initSceneStatePointLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(0.f, 100.f, 100.f), XMFLOAT3(.432f, .00056f, .000010f), true,
        true, true
    );

    std::unique_ptr<PointLightsBuffer> pointLightsBuffer =
        std::make_unique<PointLightsBuffer>();
    pointLightsBuffer->pointLights = &this->sceneState.pointLights;
    pointLightsBuffer->init(this->sceneState.graphicsManager);
    this->sceneState.pointLightsBuffer = std::move(pointLightsBuffer);
}

void State::initSceneStateCamera()
{
    std::unique_ptr<CameraBuffer> cameraBuffer =
        std::make_unique<CameraBuffer>();
    cameraBuffer->camera = &this->sceneState.camera;
    cameraBuffer->init(this->sceneState.graphicsManager);
    this->sceneState.cameraBuffer = std::move(cameraBuffer);
}

void State::initSceneState(
    D3D *graphicsManager, ShaderManager *shaderManager,
    TextureManager *textureManager
)
{
    this->sceneState.graphicsManager = graphicsManager;
    this->sceneState.shaderManager = shaderManager;
    this->sceneState.textureManager = textureManager;
    this->sceneState.clearColour = {0.392f, 0.584f, 0.929f, 1.0f};

    this->initSceneStateSampler();
    this->initSceneStateMeshes();
    this->initSceneStateAmbientLight();
    this->initSceneStatePointLights();
    this->initSceneStateCamera();
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
    D3D *graphicsManager, float *fps, AmbientLight *ambientLight,
    std::vector<std::unique_ptr<PointLight>> *pointLights
)
{
    this->debugSceneState.graphicsManager = graphicsManager;
    this->debugSceneState.fps = fps;

    this->debugSceneState.ambientLight = ambientLight;
    this->debugSceneState.pointLights = pointLights;
}
