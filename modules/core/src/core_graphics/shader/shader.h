#pragma once

#include "core_graphics/buffer/base.h"
#include "core_graphics/sampler/base.h"
#include "core_graphics/texture/base.h"
#include <optional>

#include <core_graphics/d3d/d3d.h>

class BaseShader
{
    public:
    virtual ~BaseShader() = default;

    std::string filepath;
    std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
        inputElementDescription = std::nullopt;
    std::optional<ComPtr<ID3D11InputLayout>> inputLayout;
    ComPtr<ID3DBlob> blob;
    std::vector<BaseBuffer *> bufferStack;
    std::vector<BaseSampler *> samplerStack;
    std::vector<BaseTexture *> textureStack;

    bool loadBlob(D3D *graphicsManager);
    bool load(D3D *graphicsManager);
    virtual bool create(D3D *graphicsManager) = 0;

    virtual bool set(D3D *graphicsManager) = 0;
};

class VertexShader : public BaseShader
{
    public:
    ComPtr<ID3D11VertexShader> shader;
    bool set(D3D *graphicsManager);
    bool create(D3D *graphicsManager);
};

class PixelShader : public BaseShader
{
    public:
    ComPtr<ID3D11PixelShader> shader;
    bool set(D3D *graphicsManager);
    bool create(D3D *graphicsManager);
};

class ShaderStack
{
    public:
    std::vector<BaseShader *> shaders;
};
