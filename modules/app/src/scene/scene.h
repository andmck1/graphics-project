#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <directxmath.h>
#include <array>

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/camera/camera.h>
#include <core_graphics/mesh/triangle.h>
#include <core_graphics/mesh/sphere.h>
#include <core_graphics/light/ambient.h>
#include <core_graphics/light/point.h>
#include <core_graphics/light/directional.h>
#include <core_graphics/mesh/quad.h>
#include <core_graphics/mesh/plane.h>
#include <core_graphics/mesh/ortho.h>
#include <core_graphics/mesh/base.h>
#include <core_graphics/texture/default.h>
#include <core_graphics/texture/file.h>
#include <core_graphics/actor/scene.h>
#include <core_graphics/sampler/base.h>
#include <core_graphics/sampler/clamp.h>
#include <core_graphics/sampler/wrap.h>
#include <core_graphics/sampler/downsampler.h>
#include <core_graphics/sampler/upsampler.h>
#include <core_utils/timer.h>
#include "core_graphics/light/spotlight.h"
#include "core_graphics/water/water.h"

#include "../shadermanager/shadermanager.h"
#include "../postprocessors/crt.h"
#include "shared_shader/sharedbuffertypes.h"

struct SceneState
{
    D3D *graphicsManager;
    bool isWireframeEnabled = false;
    ShaderManagerState *shaderManagerState;
    std::string vertexShaderDirectory;
    std::string pixelShaderDirectory;
    std::array<float, 4> clearColour = {0.f, 0.f, 0.f, 0.f};

    ClampSampler clampSampler = ClampSampler();
    WrapSampler wrapSampler = WrapSampler();
    DownSampler downSampler = DownSampler();
    UpSampler upSampler = UpSampler();

    std::vector<std::unique_ptr<SceneActor>> actors;

    AmbientLight *ambientLight;
    std::vector<PointLight *> pointLights;
    std::vector<SpotLight *> spotLights;
    std::vector<DirectionalLight *> directionalLights;

    Camera *camera;
    Water *water;

    SharedBufferTypes::CRTBufferType crtSettings =
        SharedBufferTypes::CRTBufferType();
    CRTPostProcessor crtPostProcessor;

    SharedBufferTypes::TessellationHullDataBufferType
        tessellationHullDataSettings =
            SharedBufferTypes::TessellationHullDataBufferType();
};

class Scene
{
    public:
    Scene(SceneState *state);
    SceneState *state;

    bool firstPass();
    bool finalPass();
    bool updateAndDraw();
};
