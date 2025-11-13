#include "state.h"

#include "core_graphics/mesh/ortho.h"
#include "core_graphics/light/ambient.h"
#include "core_graphics/mesh/quad.h"
#include "core_graphics/texture/default.h"
#include "core_graphics/texture/file.h"
#include "core_graphics/water/water.h"
#include "core_utils/timer.h"
#include "shared_shader/sharedbuffertypes.h"

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

    // Vertex
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
    this->shaderManagerState.depthVertexShader =
        this->initShaderManagerStateShader<DepthVertexShader>(
            "./build/hlsl/vertex/depth.cso", inputElementDescription
        );

    // Pixel
    this->shaderManagerState.lightingPixelShader =
        this->initShaderManagerStateShader<LightingPixelShader>(
            "./build/hlsl/pixel/lighting.cso"
        );

    this->shaderManagerState.textureOnlyPixelShader =
        this->initShaderManagerStateShader<TextureOnlyPixelShader>(
            "./build/hlsl/pixel/textureOnly.cso"
        );

    this->shaderManagerState.crtPixelShader =
        this->initShaderManagerStateShader<CRTPixelShader>(
            "./build/hlsl/pixel/crt.cso"
        );
    this->shaderManagerState.depthPixelShader =
        this->initShaderManagerStateShader<DepthPixelShader>(
            "./build/hlsl/pixel/depth.cso", inputElementDescription
        );
}

void State::initShaderManagerStateShaderStacks()
{
    this->shaderManagerState.defaultShaderStack.shaders =
        std::vector<BaseShader *>(
            {&this->shaderManagerState.standardVertexShader,
             &this->shaderManagerState.lightingPixelShader}
        );
    this->shaderManagerState.textureOnlyShaderStack.shaders =
        std::vector<BaseShader *>(
            {&this->shaderManagerState.standardVertexShader,
             &this->shaderManagerState.textureOnlyPixelShader}
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
    this->shaderManagerState.depthShaderStack.shaders =
        std::vector<BaseShader *>(
            {&this->shaderManagerState.depthVertexShader,
             &this->shaderManagerState.depthPixelShader}
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
    this->shaderManagerState.depthVertexShader.bufferStack = {
        &this->shaderManagerState.matrixBuffer,
    };

    this->shaderManagerState.lightingPixelShader.bufferStack = {
        &this->shaderManagerState.ambientLightBuffer,
        &this->shaderManagerState.directionalLightsBuffer,
        &this->shaderManagerState.pointLightsBuffer,
        &this->shaderManagerState.spotLightsBuffer
    };
    this->shaderManagerState.textureOnlyPixelShader.bufferStack = {};
    this->shaderManagerState.depthPixelShader.bufferStack = {};
    this->shaderManagerState.crtPixelShader.bufferStack = {
        &this->shaderManagerState.crtBuffer
    };
}

void State::setShaderManagerStateShaderBufferData()
{
    spdlog::debug("Setting shader manager state shader buffer data");
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
    this->shaderManagerState.spotLightsBuffer.setData(
        &this->sceneState.spotLights
    );
    this->shaderManagerState.waveDataBuffer.setData(
        &this->appState.timer->totalTime, &this->sceneState.water->magnitude,
        &this->sceneState.water->waveNumber,
        &this->sceneState.water->angularVelocity
    );
    this->shaderManagerState.crtBuffer.setData(
        &this->sceneState.crtSettings, &this->appState.timer->totalTime
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
    // static const uint32_t texturePixelColour = 0xFF0000;
    static const uint32_t texturePixelColour = 0xE0E0E0;
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

    this->sceneState.downSampler = DownSampler();
    this->sceneState.downSampler.init(this->sceneState.graphicsManager);

    this->sceneState.upSampler = UpSampler();
    this->sceneState.upSampler.init(this->sceneState.graphicsManager);
}

template <typename MeshType, typename SceneActorType>
void State::initSceneStateActor(
    ShaderStack *defaultShaderStack, BaseTexture *texture, BaseSampler *sampler,
    BaseTexture *heightMap, BaseSampler *heightMapSampler,
    std::array<float, 3> initScaling, std::array<float, 3> initRotation,
    std::array<float, 3> initTranslation
)
{
    spdlog::debug("Initialising scene state mesh");

    this->sceneState.actors.push_back(std::make_unique<SceneActorType>());
    auto *sceneActorPtr = this->sceneState.actors.back().get();
    sceneActorPtr->mesh = MeshType(this->sceneState.graphicsManager);
    sceneActorPtr->defaultShaderStack = defaultShaderStack;
    sceneActorPtr->currentShaderStack = defaultShaderStack;
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

    this->initSceneStateActor<QuadMesh, SceneActor>(
        &this->shaderManagerState.defaultShaderStack,
        &this->textureManagerState.defaultTexture,
        &this->sceneState.clampSampler, nullptr, nullptr, {100.f, 100.f, 100.f},
        {XMConvertToRadians(-90.f), 0.f, 0.f}, {0.f, -10.f, 0.f}
    );

    this->initSceneStateActor<PlaneMesh, SceneActor>(
        &this->shaderManagerState.heightMappedObjectShaderStack,
        &this->textureManagerState.mountainsHeightMap,
        &this->sceneState.clampSampler,
        &this->textureManagerState.mountainsHeightMap,
        &this->sceneState.wrapSampler, {1.f, 1.f, 1.f}, {0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f}
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
    pointLightPtr->defaultShaderStack =
        &this->shaderManagerState.textureOnlyShaderStack;
    pointLightPtr->currentShaderStack =
        &this->shaderManagerState.textureOnlyShaderStack;
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
        XMFLOAT3(0.f, 10.f, -20.f), XMFLOAT3(.1f, .01f, 0.01f), true, true, true
    );
    this->initSceneStatePointLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(0.f, 10.f, 20.f), XMFLOAT3(.1f, .01f, 0.01f), true, true, true
    );
}

void State::initSceneStateSpotLight(
    XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
    float initSpecularPower, XMFLOAT3 initWorldPosition,
    XMFLOAT3 initDirectionVector, float initConeAngle,
    XMFLOAT3 initAttenuationCoefficients, bool initIsDiffuseEnabled,
    bool initIsSpecularEnabled, bool initIsAttenuationEnabled
)
{
    spdlog::debug("Initialising scene state spot light");
    this->sceneState.actors.push_back(std::make_unique<SpotLight>());
    SpotLight *spotLightPtr =
        dynamic_cast<SpotLight *>(this->sceneState.actors.back().get());
    this->sceneState.spotLights.push_back(spotLightPtr);

    spotLightPtr->mesh = SphereMesh(this->sceneState.graphicsManager);
    spotLightPtr->defaultShaderStack =
        &this->shaderManagerState.textureOnlyShaderStack;
    spotLightPtr->currentShaderStack =
        &this->shaderManagerState.textureOnlyShaderStack;
    spotLightPtr->texture = &this->textureManagerState.lightDebugTexture;
    spotLightPtr->textureSampler = &this->sceneState.clampSampler;

    spotLightPtr->diffuseColour[0] = initDiffuseColour.x;
    spotLightPtr->diffuseColour[1] = initDiffuseColour.y;
    spotLightPtr->diffuseColour[2] = initDiffuseColour.z;
    spotLightPtr->specularColour[0] = initSpecularColour.x;
    spotLightPtr->specularColour[1] = initSpecularColour.y;
    spotLightPtr->specularColour[2] = initSpecularColour.z;
    spotLightPtr->specularPower = initSpecularPower;
    spotLightPtr->worldPosition[0] = initWorldPosition.x;
    spotLightPtr->worldPosition[1] = initWorldPosition.y;
    spotLightPtr->worldPosition[2] = initWorldPosition.z;
    spotLightPtr->directionVector[0] = initDirectionVector.x;
    spotLightPtr->directionVector[1] = initDirectionVector.y;
    spotLightPtr->directionVector[2] = initDirectionVector.z;
    spotLightPtr->coneAngle = initConeAngle;
    spotLightPtr->attenuationCoefficients[0] = initAttenuationCoefficients.x;
    spotLightPtr->attenuationCoefficients[1] = initAttenuationCoefficients.y;
    spotLightPtr->attenuationCoefficients[2] = initAttenuationCoefficients.z;
    spotLightPtr->isDiffuseEnabled = initIsDiffuseEnabled;
    spotLightPtr->isSpecularEnabled = initIsSpecularEnabled;
    spotLightPtr->isAttenuationEnabled = initIsAttenuationEnabled;

    spotLightPtr->isUpdateEnabled = true;
    spotLightPtr->isDrawEnabled = true;

    spotLightPtr->shadowMap.graphicsManager = this->sceneState.graphicsManager;
    spotLightPtr->shadowMap.width = 1024;
    spotLightPtr->shadowMap.height = 1024;
    spotLightPtr->shadowMap.init();
}

void State::initSceneStateSpotLights()
{
    spdlog::debug("Initialising scene spot lights");
    this->initSceneStateSpotLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(-20.f, 10.f, 0.f), XMFLOAT3(-1.f, -1.f, -1.f), 45.f,
        XMFLOAT3(.1f, .01f, 0.01f), true, true, true
    );
    this->initSceneStateSpotLight(
        XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT3(1.f, 1.f, 1.f), 10.f,
        XMFLOAT3(20.f, 10.f, 0.f), XMFLOAT3(-1.f, -1.f, -1.f), 45.f,
        XMFLOAT3(.1f, .01f, 0.01f), true, true, true
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

void State::createRenderTexture(
    BasePostProcessor *postProcessor, int textureWidth, int textureHeight,
    float screenNear, float screenFar
)
{
    postProcessor->renderTextures.push_back(std::make_unique<RenderTexture>());
    RenderTexture *renderTexturePtr =
        postProcessor->renderTextures.back().get();
    renderTexturePtr->graphicsManager = this->sceneState.graphicsManager;
    renderTexturePtr->textureWidth = textureWidth;
    renderTexturePtr->textureHeight = textureHeight;
    renderTexturePtr->screenNear = screenNear;
    renderTexturePtr->screenFar = screenFar;
    renderTexturePtr->init();
}

void State::addRenderStage(
    BasePostProcessor *postProcessor, bool isLastStage,
    PixelShader *pixelShader, BaseSampler *sampler,
    std::optional<int> inputRenderTextureWidth,
    std::optional<int> inputRenderTextureHeight,
    std::optional<int> outputRenderTextureWidth,
    std::optional<int> outputRenderTextureHeight
)
{
    postProcessor->renderStages.push_back(std::make_unique<RenderStage>());
    RenderStage *renderStagePtr = postProcessor->renderStages.back().get();

    RenderTexture *initialRenderTexturePtr;
    if (postProcessor->renderStages.size() == 1)
    {
        this->createRenderTexture(
            postProcessor, inputRenderTextureWidth.value(),
            inputRenderTextureHeight.value(), 0.1f, 5000.f
        );
    }
    initialRenderTexturePtr = postProcessor->renderTextures.back().get();

    std::optional<RenderTexture *> nextRenderTexturePtr = std::nullopt;
    if (!isLastStage)
    {
        this->createRenderTexture(
            postProcessor, outputRenderTextureWidth.value(),
            outputRenderTextureHeight.value(), 0.1f, 5000.f
        );
        nextRenderTexturePtr = postProcessor->renderTextures.back().get();
        renderStagePtr->mesh = OrthoMesh(
            this->sceneState.graphicsManager, outputRenderTextureHeight.value(),
            outputRenderTextureWidth.value(), 0, 0
        );
    }
    else
    {
        renderStagePtr->mesh = OrthoMesh(
            this->sceneState.graphicsManager, this->windowState.height,
            this->windowState.width, 0, 0
        );
    }

    renderStagePtr->inputRenderTexture = initialRenderTexturePtr;
    renderStagePtr->outputRenderTexture = nextRenderTexturePtr;
    renderStagePtr->textureSampler = sampler;
    renderStagePtr->clearColour = this->sceneState.clearColour;
    renderStagePtr->camera = this->sceneState.camera;
    renderStagePtr->shaderManagerState = &shaderManagerState;
    renderStagePtr->pixelShader = pixelShader;
}

void State::initSceneStatePostProcessors()
{
    spdlog::debug("Inititalising scene state post processors");
    this->sceneState.crtPostProcessor = CRTPostProcessor();
    CRTPostProcessor *crtPostProcessorPtr = &this->sceneState.crtPostProcessor;

    SharedBufferTypes::CRTBufferType *crtSettingsPtr =
        &this->sceneState.crtSettings;

    crtSettingsPtr->isVHSWiggleEnabled = XMINT4(1, 0, 0, 0);
    crtSettingsPtr->isCurvingEnabled = XMINT4(1, 0, 0, 0);
    crtSettingsPtr->isChromaticAberrationEnabled = XMINT4(1, 0, 0, 0);
    crtSettingsPtr->isGrainEnabled = XMINT4(1, 0, 0, 0);
    crtSettingsPtr->isRGBGridEnabled = XMINT4(1, 0, 0, 0);
    crtSettingsPtr->isScanlinesEnabled = XMINT4(0, 0, 0, 0);
    crtSettingsPtr->isVingetteEnabled = XMINT4(1, 0, 0, 0);

    crtSettingsPtr->brightnessMultiplier = XMFLOAT4(1.f, 0.f, 0.f, 0.f);
    crtSettingsPtr->grainStrength = XMFLOAT4(0.2f, 0.f, 0.f, 0.f);
    crtSettingsPtr->curvePower = XMFLOAT4(1.01f, 0.f, 0.f, 0.f);
    crtSettingsPtr->vingetteStrength = XMFLOAT4(0.5f, 0.f, 0.f, 0.f);
    crtSettingsPtr->scanlinesInterval = XMINT4(2, 0, 0, 0);
    crtSettingsPtr->scanlinesOpacity = XMFLOAT4(1.f, 0.f, 0.f, 0.f);
    crtSettingsPtr->scanlinesThickness = XMINT4(2, 0, 0, 0);
    crtSettingsPtr->chromaticAberrationStrength = XMFLOAT4(2.f, 0.f, 0.f, 0.f);
    crtSettingsPtr->vhsWiggle = XMFLOAT4(0.002f, 0.f, 0.f, 0.f);

    this->addRenderStage(
        crtPostProcessorPtr, false,
        &this->shaderManagerState.textureOnlyPixelShader,
        &this->sceneState.downSampler, this->windowState.width,
        this->windowState.height, this->windowState.width / 2,
        this->windowState.height / 2
    );
    this->addRenderStage(
        crtPostProcessorPtr, false, &this->shaderManagerState.crtPixelShader,
        &this->sceneState.upSampler, std::nullopt, std::nullopt,
        this->windowState.width / 2, this->windowState.height / 2
    );
    this->addRenderStage(
        crtPostProcessorPtr, true,
        &this->shaderManagerState.textureOnlyPixelShader,
        &this->sceneState.upSampler
    );
}

void State::initSceneStateActorShadowMaps()
{
    for (SpotLight *spotLight : this->sceneState.spotLights)
    {
        for (const std::unique_ptr<SceneActor> &sceneActor :
             this->sceneState.actors)
        {
            sceneActor->shadowMaps.push_back(&spotLight->shadowMap);
        }
    }
}

void State::initSceneState(
    D3D *graphicsManager, ShaderManagerState *shaderManagerState
)
{
    spdlog::debug("Initialising scene state");
    this->sceneState.graphicsManager = graphicsManager;
    this->sceneState.shaderManagerState = shaderManagerState;
    this->sceneState.clearColour = {0.392f, 0.584f, 0.929f, 1.0f};

    this->initSceneStateSampler();
    this->initSceneStateActors();
    this->initSceneStateAmbientLight();
    this->initSceneStatePointLights();
    this->initSceneStateSpotLights();
    this->initSceneStateDirectionalLights();
    this->initSceneStateCamera();
    this->initSceneStateWater();
    this->initSceneStatePostProcessors();
    this->initSceneStateActorShadowMaps();

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
    this->shaderManagerState.spotLightsBuffer.init(
        this->shaderManagerState.graphicsManager
    );
    this->shaderManagerState.waveDataBuffer.init(
        this->shaderManagerState.graphicsManager
    );
    this->shaderManagerState.crtBuffer.init(
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
    D3D *graphicsManager, float *fps, bool *isWireframeEnabled,
    AmbientLight *ambientLight, std::vector<PointLight *> *pointLights,
    std::vector<SpotLight *> *spotLights,
    std::vector<DirectionalLight *> *directionalLights, Water *water,
    SharedBufferTypes::CRTBufferType *crtSettings, bool *isCRTEnabled
)
{
    spdlog::debug("Initialising debug scene state");
    this->debugSceneState.graphicsManager = graphicsManager;
    this->debugSceneState.fps = fps;
    this->debugSceneState.isWireframeEnabled = isWireframeEnabled;

    this->debugSceneState.ambientLight = ambientLight;
    this->debugSceneState.pointLights = pointLights;
    this->debugSceneState.spotLights = spotLights;
    this->debugSceneState.directionalLights = directionalLights;
    this->debugSceneState.water = water;
    this->debugSceneState.crtSettings = crtSettings;

    this->debugSceneState.isCRTEnabled = isCRTEnabled;
}
