#pragma once

#include <core_graphics/mesh/base.h>
#include <core_graphics/shader/shader.h>
#include <core_graphics/texture/base.h>
#include <core_graphics/sampler/base.h>

#include <memory>

class BaseSceneMesh
{
    public:
    virtual ~BaseSceneMesh() = default;
    std::unique_ptr<BaseMesh> mesh;
    XMMATRIX worldMatrix;
    XMFLOAT3 worldPosition;
    XMFLOAT3 rotation;
    XMFLOAT3 scaling;
    ShaderStack *shaderStack;
    BaseTexture *texture;
    BaseSampler *sampler;
};

class BaseScene
{
    public:
    virtual ~BaseScene() = default;
};
