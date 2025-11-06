#pragma once

#include <optional>

#include <core_graphics/d3d/d3d.h>

enum class ShaderType
{
    VERTEX,
    PIXEL
};

struct BaseShader
{
    virtual ~BaseShader() = default;

    std::string filepath;
    ShaderType type;
    std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
        inputElementDescription = std::nullopt;
    ComPtr<ID3DBlob> blob;

    bool loadBlob(D3D *graphicsManager);
};

struct VertexShader : BaseShader
{
    ComPtr<ID3D11VertexShader> shader;
};

struct PixelShader : BaseShader
{
    ComPtr<ID3D11PixelShader> shader;
};

struct ShaderStack
{
    std::vector<BaseShader*> shaders;
};
