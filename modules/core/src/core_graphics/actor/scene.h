#pragma once

#include "core_graphics/actor/base.h"
#include <core_graphics/mesh/base.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/texture/base.h>
#include <core_graphics/sampler/base.h>
#include <optional>

class SceneActor : public BaseActor
{
    public:
    SceneActor();

    std::optional<BaseMesh> mesh;

    ShaderStack *shaderStack = nullptr;
    BaseTexture *texture = nullptr;
    BaseSampler *textureSampler = nullptr;
    BaseTexture *heightMap = nullptr;
    BaseSampler *heightMapSampler = nullptr;

    void update() override;
    bool drawMesh(D3D *graphicsManager);
};
