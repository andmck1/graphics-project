#pragma once

#include "core_graphics/buffer/buffer.h"
#include <optional>

#include <core_graphics/d3d/d3d.h>

enum class EnumShaderType
{
    VERTEX,
    PIXEL
};

class BaseShader
{
    public:
    virtual ~BaseShader() = default;

    std::string filepath;
    EnumShaderType type;
    std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
        inputElementDescription = std::nullopt;
    ComPtr<ID3DBlob> blob;
    std::vector<Buffer> bufferStack;

    bool loadBlob(D3D *graphicsManager);
};

class VertexShader : public BaseShader
{
    public:
    ComPtr<ID3D11VertexShader> shader;
};

class PixelShader : public BaseShader
{
    public:
    ComPtr<ID3D11PixelShader> shader;
    std::vector<PixelBuffer> bufferStack;
};

class ShaderStack
{
    public:
    std::vector<BaseShader *> shaders;
};
