#pragma once

#include "core_graphics/light/directional.h"
#include "core_graphics/shader/shader.h"
#include "core_graphics/water/water.h"
#include "core_window/window.h"
#include "core_graphics/d3d/d3d.h"
#include "../shadermanager/shadermanager.h"
#include "../texture/textureManager.h"
#include "../scene/scene.h"
#include "../input/input.h"
#include "../scene/debugScene.h"
#include "../app/app.h"
#include <DirectXMathMatrix.inl>

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
        D3D *graphicsManager, ShaderManager *shaderManager,
        TextureManager *textureManager
    );
    void initAppInputState(Window *window, Scene *scene);
    void initAppState(AppInput *appInput);
    void initDebugSceneState(
        D3D *graphicsManager, float *fps, AmbientLight *ambientLight,
        std::vector<std::unique_ptr<PointLight>> *pointLights,
        std::vector<std::unique_ptr<DirectionalLight>> *directionalLights,
        Water *water
    );

    private:
    void initShaderManagerStateShaderStacks();
    void initSceneStateSampler();
    template <typename MeshType>
    void initSceneStateMesh(
        ShaderStack *shaderStack, BaseTexture *texture, BaseSampler *sampler,
        BaseTexture *heightMap = nullptr,
        BaseSampler *heightMapSampler = nullptr,
        XMMATRIX initScaling = XMMatrixIdentity(),
        XMMATRIX initRotation = XMMatrixIdentity(),
        XMMATRIX initTranslation = XMMatrixIdentity()

    );
    void initSceneStateMeshes();
    void initSceneStateAmbientLight();
    void initSceneStatePointLights();
    void initSceneStatePointLight(
        XMFLOAT3 initDiffuseColour, XMFLOAT3 initSpecularColour,
        float initSpecularPower, XMFLOAT3 initWorldPosition,
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
};
