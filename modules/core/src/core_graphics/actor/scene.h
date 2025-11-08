#pragma once

#include "core_graphics/actor/base.h"
#include <core_graphics/mesh/base.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/texture/base.h>
#include <core_graphics/sampler/base.h>

class SceneActor : public BaseActor
{
    public:
    SceneActor();

    BaseMesh mesh;

    ShaderStack *shaderStack;
    BaseTexture *texture;
    BaseSampler *textureSampler;
    BaseTexture *heightMap;
    BaseSampler *heightMapSampler;

    void update() override;
};
