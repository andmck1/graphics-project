#pragma once

#include "core_graphics/light/directional.h"
#include "core_graphics/light/spotlight.h"
#include "core_graphics/sampler/base.h"
#include "core_graphics/shader/shader.h"
#include "core_graphics/water/water.h"
#include "core_window/window.h"
#include "core_graphics/d3d/d3d.h"
#include "../shadermanager/shadermanager.h"
#include "../texturemanager/texturemanager.h"
#include "../scene/scene.h"
#include "../input/input.h"
#include "../scene/debugScene.h"
#include "../app/app.h"
#include <DirectXMathMatrix.inl>
#include <optional>

class State
{
    public:
    WindowState windowState = WindowState();
    D3DAppState d3dAppState = D3DAppState();
    ShaderManagerState shaderManagerState = ShaderManagerState();
    TextureManagerState textureManagerState = TextureManagerState();
    SceneState sceneState = SceneState();
    AppInputState appInputState = AppInputState();
    AppState appState = AppState();
    DebugSceneState debugSceneState = DebugSceneState();

    void initWindowState();
    void initD3DAppState(Window *window);
    void initShaderManagerState(D3D *graphicsManager);
    void initTextureManagerState(D3D *graphicsManager);
    void initSceneState(
        D3D *graphicsManager, ShaderManagerState *shaderManagerState
    );
    void setShaderManagerStateShaderBufferData();
    void initAppInputState(Window *window, Scene *scene, Timer *timer);
    void initAppState();
    void initDebugSceneState(
        D3D *graphicsManager, float *fps, bool *isWireframeEnabled,
        bool *isNormalsAsColourEnabled, AmbientLight *ambientLight,
        std::vector<PointLight *> *pointLights,
        std::vector<SpotLight *> *spotLights,
        std::vector<DirectionalLight *> *directionalLights, Water *water,
        SharedBufferTypes::CRTBufferType *crtSettings, bool *isCRTEnabled,
        SharedBufferTypes::TessellationHullDataBufferType *tessellationSettings
    );

    private:
    template <typename ShaderType>
    ShaderType initShaderManagerStateShader(
        std::string filepath,
        std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
            inputElementDescription = std::nullopt
    );

    void initShaderManagerStateShaders();
    void initShaderManagerStateShaderBuffers();
    void initShaderManagerStateShaderStacks();
    void initSceneStateSampler();

    void initSceneStatePostProcessors();
    template <typename MeshType, typename SceneActorType>
    void initSceneStateActor(
        ShaderStack *defaultShaderStack, ShaderStack *depthShaderStack,
        BaseTexture *texture, BaseSampler *sampler,
        BaseTexture *heightMap = nullptr,
        BaseSampler *heightMapSampler = nullptr,
        std::array<float, 3> initScaling = {1.f, 1.f, 1.f},
        std::array<float, 3> initRotation = {0.f, 0.f, 0.f},
        std::array<float, 3> initTranslation = {0.f, 0.f, 0.f}
    );

    void initSceneStateActors();
    void initSceneStateAmbientLight();
    void initSceneStatePointLights();
    void initSceneStatePointLight(
        XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
        float initSpecularPower, XMFLOAT3 initWorldPosition,
        XMFLOAT3 initAttenuationCoefficients, bool initIsDiffuseEnabled,
        bool initIsSpecularEnabled, bool initIsAttenuationEnabled
    );
    void initSceneStateSpotLights();
    void initSceneStateSpotLight(
        XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
        float initSpecularPower, XMFLOAT3 initWorldPosition,
        XMFLOAT3 initDirectionVector, float initConeAngle,
        XMFLOAT3 initAttenuationCoefficients, bool initIsDiffuseEnabled,
        bool initIsSpecularEnabled, bool initIsAttenuationEnabled
    );
    void initSceneStateDirectionalLights();
    void initSceneStateDirectionalLight(
        XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
        float initSpecularPower, XMFLOAT3 initWorldPosition,
        XMFLOAT3 initDirectionVector, XMFLOAT3 initAttenuationCoefficients,
        bool initIsDiffuseEnabled, bool initIsSpecularEnabled,
        bool initIsAttenuationEnabled
    );
    void initSceneStateCamera();
    void initSceneStateWater();
    void initSceneStateActorShadowMaps();
    void initSceneStateTessellationSettings();
    void createRenderTexture(
        BasePostProcessor *postProcessor, int textureWidth, int textureHeight,
        float screenNear, float screenFar
    );
    void addRenderStage(
        BasePostProcessor *postProcessor, bool isLastStage,
        PixelShader *pixelShader, BaseSampler *sampler,
        std::optional<int> inputRenderTextureWidth = std::nullopt,
        std::optional<int> inputRenderTextureHeight = std::nullopt,
        std::optional<int> outputRenderTextureWidth = std::nullopt,
        std::optional<int> outputRenderTextureHeight = std::nullopt
    );
};
