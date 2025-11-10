#include "state.h"

#include "core_graphics/light/ambient.h"
#include "core_graphics/texture/default.h"
#include "core_graphics/texture/file.h"
#include "core_graphics/water/water.h"
#include "core_utils/timer.h"

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
    std::string filepath,
    std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>> inputElementDescription
)
{
    ShaderType shader = ShaderType();
    shader.filepath = filepath;
    shader.inputElementDescription = inputElementDescription;
    shader.load(this->shaderManagerState.graphicsManager);
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
        this->initShaderManagerStateShader<StandardVertexShader>(
            "./build/hlsl/vertex/standard.cso", inputElementDescription
        );

    this->shaderManagerState.heightMapVertexShader =
        this->initShaderManagerStateShader<HeightMapVertexShader>(
            "./build/hlsl/vertex/heightMap.cso", inputElementDescription
        );

    this->shaderManagerState.waveVertexShader =
        this->initShaderManagerStateShader<WaveVertexShader>(
            "./build/hlsl/vertex/wave.cso", inputElementDescription
        );

    this->shaderManagerState.lightingPixelShader =
        this->initShaderManagerStateShader<LightingPixelShader>(
            "./build/hlsl/pixel/lighting.cso"
        );

    this->shaderManagerState.lightSourcePixelShader =
        this->initShaderManagerStateShader<LightSourcePixelShader>(
            "./build/hlsl/pixel/lightSource.cso"
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
        &this->shaderManagerState.matrixBuffer,
        &this->shaderManagerState.cameraBuffer
    };
    this->shaderManagerState.heightMapVertexShader.bufferStack = {
        &this->shaderManagerState.matrixBuffer,
        &this->shaderManagerState.cameraBuffer
    };
    this->shaderManagerState.waveVertexShader.bufferStack = {
        &this->shaderManagerState.matrixBuffer,
        &this->shaderManagerState.cameraBuffer,
        &this->shaderManagerState.waveDataBuffer
    };
    this->shaderManagerState.lightingPixelShader.bufferStack = {
        &this->shaderManagerState.ambientLightBuffer,
        &this->shaderManagerState.directionalLightsBuffer,
        &this->shaderManagerState.pointLightsBuffer
    };
    this->shaderManagerState.lightSourcePixelShader.bufferStack = {};
}

void State::setShaderManagerStateShaderBufferData()
{
    this->shaderManagerState.cameraBuffer.setData(this->sceneState.camera);

    this->shaderManagerState.ambientLightBuffer.setData(
        this->sceneState.ambientLight
    );
    this->shaderManagerState.directionalLightsBuffer.setData(
        &this->sceneState.directionalLights
    );
    this->shaderManagerState.pointLightsBuffer.setData(
        &this->sceneState.pointLights
    );
    this->shaderManagerState.waveDataBuffer.setData(
        &this->appState.timer->totalTime, &this->sceneState.water->magnitude,
        &this->sceneState.water->waveNumber,
        &this->sceneState.water->angularVelocity
    );
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
            std::filesystem::path("./textures/light_debug.jpg")
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

template <typename MeshType, typename SceneActorType>
void State::initSceneStateActor(
    ShaderStack *shaderStack, BaseTexture *texture, BaseSampler *sampler,
    BaseTexture *heightMap, BaseSampler *heightMapSampler,
    std::array<float, 3> initScaling, std::array<float, 3> initRotation,
    std::array<float, 3> initTranslation
)
{
    spdlog::debug("Initialising scene state mesh");

    this->sceneState.actors.push_back(std::make_unique<SceneActorType>());
    auto *sceneActorPtr = this->sceneState.actors.back().get();
    sceneActorPtr->mesh = MeshType(this->sceneState.graphicsManager);
    sceneActorPtr->shaderStack = shaderStack;
    sceneActorPtr->texture = texture;
    sceneActorPtr->textureSampler = sampler;
    sceneActorPtr->heightMap = heightMap;
    sceneActorPtr->heightMapSampler = heightMapSampler;
    sceneActorPtr->isUpdateEnabled = true;
    sceneActorPtr->isDrawEnabled = true;

    sceneActorPtr->moveTo(
        initTranslation[0], initTranslation[1], initTranslation[2]
    );
    sceneActorPtr->scaleBy(initScaling[0], initScaling[1], initScaling[2]);
    sceneActorPtr->rotateBy(initRotation[0], initRotation[1], initRotation[2]);
}

void State::initSceneStateActors()
{
    spdlog::debug("Initialising scene state meshes");
    this->initSceneStateActor<SphereMesh, SceneActor>(
        &this->shaderManagerState.defaultShaderStack,
        &this->textureManagerState.moonTexture, &this->sceneState.clampSampler,
        nullptr, nullptr, {1.f, 1.f, 1.f}, {0.f, 0.f, 0.f}, {-3.f, 0.f, 0.f}
    );

    this->initSceneStateActor<PlaneMesh, SceneActor>(
        &this->shaderManagerState.heightMappedObjectShaderStack,
        &this->textureManagerState.mountainsHeightMap,
        &this->sceneState.clampSampler,
        &this->textureManagerState.mountainsHeightMap,
        &this->sceneState.wrapSampler, {1.f, 1.f, 1.f}, {0.f, 0.f, 0.f},
        {-3.f, 0.f, 0.f}
    );
}

void State::initSceneStateAmbientLight()
{
    spdlog::debug("Initialising scene state ambient light");
    this->sceneState.actors.push_back(std::make_unique<AmbientLight>());
    AmbientLight *ambientLightPtr =
        dynamic_cast<AmbientLight *>(this->sceneState.actors.back().get());
    this->sceneState.ambientLight = ambientLightPtr;

    ambientLightPtr->colour[0] = 0.1f;
    ambientLightPtr->colour[1] = 0.1f;
    ambientLightPtr->colour[2] = 0.1f;
    ambientLightPtr->isEnabled = true;
    ambientLightPtr->isDrawEnabled = false;
}

void State::initSceneStatePointLight(
    XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
    float initSpecularPower, XMFLOAT3 initWorldPosition,
    XMFLOAT3 initAttenuationCoefficients, bool initIsDiffuseEnabled,
    bool initIsSpecularEnabled, bool initIsAttenuationEnabled
)
{
    spdlog::debug("Initialising scene state point light");
    this->sceneState.actors.push_back(std::make_unique<PointLight>());
    PointLight *pointLightPtr =
        dynamic_cast<PointLight *>(this->sceneState.actors.back().get());
    this->sceneState.pointLights.push_back(pointLightPtr);

    pointLightPtr->mesh = SphereMesh(this->sceneState.graphicsManager);
    pointLightPtr->shaderStack =
        &this->shaderManagerState.lightSourceShaderStack;
    pointLightPtr->texture = &this->textureManagerState.lightDebugTexture;
    pointLightPtr->textureSampler = &this->sceneState.clampSampler;

    pointLightPtr->diffuseColour[0] = initDiffuseColour.x;
    pointLightPtr->diffuseColour[1] = initDiffuseColour.y;
    pointLightPtr->diffuseColour[2] = initDiffuseColour.z;
    pointLightPtr->specularColour[0] = initSpecularColour.x;
    pointLightPtr->specularColour[1] = initSpecularColour.y;
    pointLightPtr->specularColour[2] = initSpecularColour.z;
    pointLightPtr->specularPower = initSpecularPower;
    pointLightPtr->worldPosition[0] = initWorldPosition.x;
    pointLightPtr->worldPosition[1] = initWorldPosition.y;
    pointLightPtr->worldPosition[2] = initWorldPosition.z;
    pointLightPtr->attenuationCoefficients[0] = initAttenuationCoefficients.x;
    pointLightPtr->attenuationCoefficients[1] = initAttenuationCoefficients.y;
    pointLightPtr->attenuationCoefficients[2] = initAttenuationCoefficients.z;
    pointLightPtr->isDiffuseEnabled = initIsDiffuseEnabled;
    pointLightPtr->isSpecularEnabled = initIsSpecularEnabled;
    pointLightPtr->isAttenuationEnabled = initIsAttenuationEnabled;

    pointLightPtr->isUpdateEnabled = true;
    pointLightPtr->isDrawEnabled = true;
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
    this->sceneState.actors.push_back(std::make_unique<DirectionalLight>());
    DirectionalLight *directionalLightPtr =
        dynamic_cast<DirectionalLight *>(this->sceneState.actors.back().get());
    this->sceneState.directionalLights.push_back(directionalLightPtr);

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

    directionalLightPtr->isUpdateEnabled = false;
    directionalLightPtr->isDrawEnabled = false;
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
}

void State::initSceneStateCamera()
{
    spdlog::debug("Initialising scene state camera");
    this->sceneState.actors.push_back(std::make_unique<Camera>());
    Camera *cameraPtr =
        dynamic_cast<Camera *>(this->sceneState.actors.back().get());
    cameraPtr->isDrawEnabled = false;
    cameraPtr->isUpdateEnabled = true;
    this->sceneState.camera = cameraPtr;
}

void State::initSceneStateWater()
{
    spdlog::debug("Initialising scene state water");
    this->initSceneStateActor<PlaneMesh, Water>(
        &this->shaderManagerState.waveDataShaderStack,
        &this->textureManagerState.defaultTexture,
        &this->sceneState.clampSampler, nullptr, nullptr, {1.f, 1.f, 1.f},
        {0.f, 0.f, 0.f}, {0.f, -250.f, 0.f}
    );
    Water *waterPtr =
        dynamic_cast<Water *>(this->sceneState.actors.back().get());
    this->sceneState.water = waterPtr;

    waterPtr->isDrawEnabled = true;
    waterPtr->magnitude = 2.f;
    waterPtr->waveNumber = 0.1f;
    waterPtr->angularVelocity = 0.5f;
}

void State::initSceneState(
    D3D *graphicsManager, ShaderManagerState *shaderManagerState
)
{
    spdlog::debug("Initialising scene state");
    this->sceneState.graphicsManager = graphicsManager;
    // TODO - recreate shader manager?
    this->sceneState.shaderManagerState = shaderManagerState;
    this->sceneState.clearColour = {0.392f, 0.584f, 0.929f, 1.0f};

    this->initSceneStateSampler();
    this->initSceneStateActors();
    this->initSceneStateAmbientLight();
    this->initSceneStatePointLights();
    this->initSceneStateDirectionalLights();
    this->initSceneStateCamera();
    this->initSceneStateWater();

    this->setShaderManagerStateShaderBufferData();

    this->shaderManagerState.matrixBuffer.init(
        this->shaderManagerState.graphicsManager
    );
    this->shaderManagerState.cameraBuffer.init(
        this->shaderManagerState.graphicsManager
    );
    this->shaderManagerState.ambientLightBuffer.init(
        this->shaderManagerState.graphicsManager
    );
    this->shaderManagerState.directionalLightsBuffer.init(
        this->shaderManagerState.graphicsManager
    );
    this->shaderManagerState.pointLightsBuffer.init(
        this->shaderManagerState.graphicsManager
    );
    this->shaderManagerState.waveDataBuffer.init(
        this->shaderManagerState.graphicsManager
    );
}

void State::initAppInputState(Window *window, Scene *scene, Timer *timer)
{
    spdlog::debug("Initialising app input state");
    this->appInputState.window = window;
    this->appInputState.scene = scene;
    this->appInputState.timer = timer;
    this->appInputState.cameraMoveSpeed = 100.f;
    this->appInputState.cameraRotateSensitivity = 0.1f;
}

void State::initAppState()
{
    spdlog::debug("Initialising app state");
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
