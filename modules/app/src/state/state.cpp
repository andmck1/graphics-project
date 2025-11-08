#include "state.h"

#include "core_graphics/light/ambient.h"
#include "core_graphics/texture/default.h"
#include "core_graphics/texture/file.h"
#include "core_graphics/water/water.h"

#include <DirectXMathMatrix.inl>
#include <directxmath.h>
#include <spdlog/spdlog.h>
#include <filesystem>

void State::initWindowState()
{
    spdlog::debug("Initialising window state");
    this->windowState.height = 1080;
    this->windowState.width = 1920;
    this->windowState.windowName = "Graphics Project";
}

void State::initD3DAppState(Window *window)
{
    spdlog::debug("Initialising d3d app state");
    this->d3dAppState.window = window;
}

template <typename ShaderType>
ShaderType State::initShaderManagerStateShader(
    std::string filepath, EnumShaderType type,
    std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>> inputElementDescription
)
{
    ShaderType shader = ShaderType();
    shader.filepath = filepath;
    shader.type = type;
    shader.inputElementDescription = inputElementDescription;
    return shader;
}

void State::initShaderManagerStateShaders()
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescription = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    this->shaderManagerState.standardVertexShader =
        this->initShaderManagerStateShader<VertexShader>(
            "./build/hlsl/vertex/standard.cso", EnumShaderType::VERTEX,
            inputElementDescription
        );

    this->shaderManagerState.heightMapVertexShader =
        this->initShaderManagerStateShader<VertexShader>(
            "./build/hlsl/vertex/heightMap.cso", EnumShaderType::VERTEX,
            inputElementDescription
        );

    this->shaderManagerState.waveVertexShader =
        this->initShaderManagerStateShader<VertexShader>(
            "./build/hlsl/vertex/wave.cso", EnumShaderType::VERTEX,
            inputElementDescription
        );

    this->shaderManagerState.lightingPixelShader =
        this->initShaderManagerStateShader<PixelShader>(
            "./build/hlsl/pixel/lighting.cso", EnumShaderType::PIXEL
        );

    this->shaderManagerState.lightSourcePixelShader =
        this->initShaderManagerStateShader<PixelShader>(
            "./build/hlsl/pixel/lightSource.cso", EnumShaderType::PIXEL
        );
}

void State::initShaderManagerStateShaderStacks()
{
    this->shaderManagerState.defaultShaderStack.shaders =
        std::vector<BaseShader *>(
            {&this->shaderManagerState.standardVertexShader,
             &this->shaderManagerState.lightingPixelShader}
        );
    this->shaderManagerState.lightSourceShaderStack.shaders =
        std::vector<BaseShader *>(
            {&this->shaderManagerState.standardVertexShader,
             &this->shaderManagerState.lightSourcePixelShader}
        );
    this->shaderManagerState.heightMappedObjectShaderStack.shaders =
        std::vector<BaseShader *>(
            {&this->shaderManagerState.heightMapVertexShader,
             &this->shaderManagerState.lightingPixelShader}
        );
    this->shaderManagerState.waveDataShaderStack.shaders =
        std::vector<BaseShader *>(
            {&this->shaderManagerState.waveVertexShader,
             &this->shaderManagerState.lightingPixelShader}
        );
}

void State::initShaderManagerStateShaderBuffers()
{
    this->shaderManagerState.standardVertexShader.bufferStack = {
        this->shaderManagerState.standardVertexShader.matrixBuffer
    };
}

void State::initShaderManagerState(D3D *graphicsManager)
{
    spdlog::debug("Initialising shader manager state");
    this->shaderManagerState.graphicsManager = graphicsManager;

    this->initShaderManagerStateShaders();
    this->initShaderManagerStateShaderStacks();
    this->initShaderManagerStateShaderBuffers();
}

void State::initTextureManagerState(D3D *graphicsManager)
{
    spdlog::debug("Initialising texture manager state");
    static const uint32_t texturePixelColour = 0xFF0000;
    this->textureManagerState.defaultTexture = DefaultTexture();
    this->textureManagerState.defaultTexture.graphicsManager = graphicsManager;
    this->textureManagerState.defaultTexture.texturePixelColour =
        texturePixelColour;

    this->textureManagerState.moonTexture = FileTexture();
    this->textureManagerState.moonTexture.graphicsManager = graphicsManager;
    this->textureManagerState.moonTexture.filepath = std::filesystem::absolute(
        std::filesystem::path("./textures/moon_heightmap.jpg")
    );

    this->textureManagerState.mountainsHeightMap = FileTexture();
    this->textureManagerState.mountainsHeightMap.graphicsManager =
        graphicsManager;
    this->textureManagerState.mountainsHeightMap.filepath =
        std::filesystem::absolute(
            std::filesystem::path("./textures/mountains_heightmap.png")
        );

    this->textureManagerState.mountainsTexture = FileTexture();
    this->textureManagerState.mountainsTexture.graphicsManager =
        graphicsManager;
    this->textureManagerState.mountainsTexture.filepath =
        std::filesystem::absolute(
            std::filesystem::path("./textures/mountains.png")
        );

    this->textureManagerState.lightDebugTexture = FileTexture();
    this->textureManagerState.lightDebugTexture.graphicsManager =
        graphicsManager;
    this->textureManagerState.lightDebugTexture.filepath =
        std::filesystem::absolute(
            std::filesystem::path("./textures/light_debug.png")
        );
}

void State::initSceneStateSampler()
{
    spdlog::debug("Initialising scene state samplers.");
    this->sceneState.clampSampler = ClampSampler();
    this->sceneState.clampSampler.init(this->sceneState.graphicsManager);

    this->sceneState.wrapSampler = WrapSampler();
    this->sceneState.wrapSampler.init(this->sceneState.graphicsManager);
}

template <typename MeshType>
void State::initSceneStateActor(
    ShaderStack *shaderStack, BaseTexture *texture, BaseSampler *sampler,
    BaseTexture *heightMap, BaseSampler *heightMapSampler, XMMATRIX initScaling,
    XMMATRIX initRotation, XMMATRIX initTranslation
)
{
    spdlog::debug("Initialising scene state mesh");

    // TODO - handle bool returns
    MeshType mesh = MeshType(this->sceneState.graphicsManager);
    std::unique_ptr<SceneActor> sceneActor = std::make_unique<SceneActor>();
    sceneActor->mesh = std::make_unique<MeshType>(mesh);
    sceneActor->shaderStack = shaderStack;
    sceneActor->texture = texture;
    sceneActor->textureSampler = sampler;
    sceneActor->heightMap = heightMap;
    sceneActor->heightMapSampler = heightMapSampler;
    this->sceneState.actors.push_back(std::move(sceneActor));
}

void State::initSceneStateActors()
{
    spdlog::debug("Initialising scene state meshes");
    this->initSceneStateActor<SphereMesh>(
        &this->shaderManagerState.defaultShaderStack,
        this->sceneState.textureManager->state->moonTexture.get(),
        &this->sceneState.clampSampler, nullptr, nullptr, XMMatrixIdentity(),
        XMMatrixIdentity(), XMMatrixTranslation(-3.f, 0.f, 0.f)
    );

    this->initSceneStateActor<PlaneMesh>(
        &this->shaderManagerState.heightMappedObjectShaderStack,
        this->sceneState.textureManager->state->mountainsHeightMap.get(),
        &this->sceneState.clampSampler,
        this->sceneState.textureManager->state->mountainsHeightMap.get(),
        &this->sceneState.wrapSampler, XMMatrixIdentity(), XMMatrixIdentity(),
        XMMatrixIdentity()
    );
}

void State::initSceneStateAmbientLight()
{
    spdlog::debug("Initialising scene state ambient light");
    this->sceneState.ambientLight = AmbientLight();
    this->sceneState.ambientLightBuffer = AmbientLightBuffer();

    this->sceneState.ambientLight.colour = std::array<float, 3>();
    this->sceneState.ambientLight.colour[0] = 0.1f;
    this->sceneState.ambientLight.colour[1] = 0.1f;
    this->sceneState.ambientLight.colour[2] = 0.1f;
    this->sceneState.ambientLight.isEnabled = true;

    this->sceneState.ambientLightBuffer.init(this->sceneState.graphicsManager);

    this->sceneState.ambientLightBuffer.ambientLight =
        &this->sceneState.ambientLight;
}

void State::initSceneStatePointLight(
    XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
    float initSpecularPower, XMFLOAT3 initWorldPosition,
    XMFLOAT3 initAttenuationCoefficients, bool initIsDiffuseEnabled,
    bool initIsSpecularEnabled, bool initIsAttenuationEnabled
)
{
    spdlog::debug("Initialising scene state point light");
    this->sceneState.actors.push_back(PointLight());
    PointLight *pointLightPtr =
        dynamic_cast<PointLight *>(&this->sceneState.actors.back());
    pointLightPtr->mesh = SphereMesh(this->sceneState.graphicsManager);

    pointLightPtr->shaderStack =
        &this->shaderManagerState.lightSourceShaderStack;
    pointLightPtr->texture =
        this->sceneState.textureManager->state->lightDebugTexture.get();
    pointLightPtr->textureSampler = &this->sceneState.clampSampler;

    this->sceneState.pointLights.push_back(pointLightPtr);
}

void State::initSceneStatePointLights()
{
    spdlog::debug("Initialising scene state point lights");
    this->initSceneStatePointLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(0.f, 0.f, 5.f), XMFLOAT3(.432f, .00056f, .000010f), true, true,
        true
    );
    this->initSceneStatePointLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(0.f, 5.f, 5.f), XMFLOAT3(.432f, .00056f, .000010f), true, true,
        true
    );

    std::unique_ptr<PointLightsBuffer> pointLightsBuffer =
        std::make_unique<PointLightsBuffer>();
    pointLightsBuffer->pointLights = &this->sceneState.pointLights;
    pointLightsBuffer->init(this->sceneState.graphicsManager);
    this->sceneState.pointLightsBuffer = PointLightsBuffer();
    this->sceneState.pointLightsBuffer.pointLights =
        &this->sceneState.pointLights;
}

void State::initSceneStateDirectionalLight(
    XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
    float initSpecularPower, XMFLOAT3 initWorldPosition,
    XMFLOAT3 initDirectionVector, XMFLOAT3 initAttenuationCoefficients,
    bool initIsDiffuseEnabled, bool initIsSpecularEnabled,
    bool initIsAttenuationEnabled
)
{
    spdlog::debug("Initialising scene state directional light");

    this->sceneState.directionalLights.push_back(DirectionalLight());
    DirectionalLight *directionalLightPtr =
        &this->sceneState.directionalLights.back();

    directionalLightPtr->diffuseColour = std::array<float, 3>();
    directionalLightPtr->diffuseColour[0] = initDiffuseColour.x;
    directionalLightPtr->diffuseColour[1] = initDiffuseColour.y;
    directionalLightPtr->diffuseColour[2] = initDiffuseColour.z;
    directionalLightPtr->specularColour = std::array<float, 3>();
    directionalLightPtr->specularColour[0] = initSpecularColour.x;
    directionalLightPtr->specularColour[1] = initSpecularColour.y;
    directionalLightPtr->specularColour[2] = initSpecularColour.z;
    directionalLightPtr->specularPower = initSpecularPower;
    directionalLightPtr->directionVector = std::array<float, 3>();
    directionalLightPtr->directionVector[0] = initDirectionVector.x;
    directionalLightPtr->directionVector[1] = initDirectionVector.y;
    directionalLightPtr->directionVector[2] = initDirectionVector.z;
    directionalLightPtr->isDiffuseEnabled = initIsDiffuseEnabled;
    directionalLightPtr->isSpecularEnabled = initIsSpecularEnabled;
}

void State::initSceneStateDirectionalLights()
{
    spdlog::debug("Initialising scene state directional lights");
    this->initSceneStateDirectionalLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(5.f, 0.f, 5.f), XMFLOAT3(-1.f, 0.f, 0.f),
        XMFLOAT3(.432f, .00056f, .000010f), true, true, true
    );
    this->initSceneStateDirectionalLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(5.f, 0.f, -5.f), XMFLOAT3(-1.f, 0.f, 0.f),
        XMFLOAT3(.432f, .00056f, .000010f), true, true, true
    );
    for (int i=0; i<direction
    directionalLightsBuffer->directionalLights =
        &this->sceneState.directionalLights;
    directionalLightsBuffer->init(this->sceneState.graphicsManager);
    this->sceneState.directionalLightsBuffer =
        std::move(directionalLightsBuffer);
}

void State::initSceneStateCamera()
{
    spdlog::debug("Initialising scene state camera");
    std::unique_ptr<CameraBuffer> cameraBuffer =
        std::make_unique<CameraBuffer>();
    cameraBuffer->camera = &this->sceneState.camera;
    cameraBuffer->init(this->sceneState.graphicsManager);
    this->sceneState.cameraBuffer = std::move(cameraBuffer);
}

void State::initSceneStateWater()
{
    spdlog::debug("Initialising scene state water");

    std::unique_ptr<Water> water = std::make_unique<Water>();
    water->magnitude = std::make_unique<float>(2.f);
    water->waveNumber = std::make_unique<float>(0.1f);
    water->angularVelocity = std::make_unique<float>(0.5f);
    this->sceneState.water = std::move(water);

    std::unique_ptr<WaveDataBuffer> waveDataBuffer =
        std::make_unique<WaveDataBuffer>();
    waveDataBuffer->totalTime = &this->sceneState.timer.totalTime;
    waveDataBuffer->magnitude = this->sceneState.water->magnitude.get();
    waveDataBuffer->waveNumber = this->sceneState.water->waveNumber.get();
    waveDataBuffer->angularVelocity =
        this->sceneState.water->angularVelocity.get();
    waveDataBuffer->init(this->sceneState.graphicsManager);
    this->sceneState.waveDataBuffer = std::move(waveDataBuffer);

    this->initSceneStateActor<PlaneMesh>(
        &this->shaderManagerState.waveDataShaderStack,
        this->sceneState.textureManager->state->defaultTexture.get(),
        this->sceneState.clampSampler.get(), nullptr, nullptr,
        XMMatrixIdentity(), XMMatrixIdentity(),
        XMMatrixTranslation(0.f, -125.f, 0.f)
    );

    this->sceneState.actors.back()->isWave = true;
}

void State::initSceneState(
    D3D *graphicsManager, ShaderManager *shaderManager,
    TextureManager *textureManager
)
{
    spdlog::debug("Initialising scene state");
    this->sceneState.graphicsManager = graphicsManager;
    this->sceneState.shaderManager = shaderManager;
    this->sceneState.textureManager = textureManager;
    this->sceneState.clearColour = {0.392f, 0.584f, 0.929f, 1.0f};

    this->initSceneStateSampler();
    this->initSceneStateActors();
    this->initSceneStateAmbientLight();
    this->initSceneStatePointLights();
    this->initSceneStateDirectionalLights();
    this->initSceneStateCamera();
    this->initSceneStateWater();
}

void State::initAppInputState(Window *window, Scene *scene)
{
    spdlog::debug("Initialising app input state");
    this->appInputState.window = window;
    this->appInputState.scene = scene;
    this->appInputState.cameraMoveSpeed = 100.f;
    this->appInputState.cameraRotateSensitivity = 0.1f;
}

void State::initAppState(AppInput *appInput)
{
    spdlog::debug("Initialising app state");
    this->appState.appInput = appInput;
}

void State::initDebugSceneState(
    D3D *graphicsManager, float *fps, AmbientLight *ambientLight,
    std::vector<PointLight *> *pointLights,
    std::vector<DirectionalLight *> *directionalLights, Water *water
)
{
    spdlog::debug("Initialising debug scene state");
    this->debugSceneState.graphicsManager = graphicsManager;
    this->debugSceneState.fps = fps;

    this->debugSceneState.ambientLight = ambientLight;
    this->debugSceneState.pointLights = pointLights;
    this->debugSceneState.directionalLights = directionalLights;
    this->debugSceneState.water = water;
}
