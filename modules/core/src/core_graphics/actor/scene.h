#pragma once

#include "core_graphics/actor/base.h"
#include "core_graphics/d3d/shadowmap.h"
#include <core_graphics/mesh/base.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/texture/base.h>
#include <core_graphics/sampler/base.h>
#include <optional>
#include <memory>

class SceneActor : public BaseActor
{
    public:
    SceneActor();

    std::optional<std::unique_ptr<BaseMesh>> mesh;

    ShaderStack *currentShaderStack = nullptr;
    ShaderStack *defaultShaderStack = nullptr;
    ShaderStack *depthShaderStack = nullptr;
    BaseTexture *texture = nullptr;
    BaseSampler *textureSampler = nullptr;
    BaseTexture *heightMap = nullptr;
    BaseSampler *heightMapSampler = nullptr;
    std::vector<ShadowMap *> shadowMaps;
    bool isUpdateEnabled = true;
    bool isDrawEnabled = true;

    void update() override;
    bool drawMesh(D3D *graphicsManager, bool isTextureSet = true);
};
