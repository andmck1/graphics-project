#pragma once

#include <optional>
#include <filesystem>

#include <core_graphics/d3d/d3d.h>


class ShaderManager
{
    public:
    ShaderManager(D3D *graphicsManager);

    enum class Shader
    {
        STANDARD_VERTEX,
        LIGHTING_PIXEL,
        END
    };

    enum class ShaderType
    {
        VERTEX,
        PIXEL
    };

    std::optional<Shader> getShaderFromFilename(std::string filename);
    std::optional<ShaderType> getShaderType(Shader shader);
    std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
    getInputElementDescription(Shader shader);
    std::optional<ComPtr<ID3DBlob>> getShaderBlob(Shader shader);
    std::optional<ComPtr<ID3D11VertexShader>>
    getLoadedVertexShader(Shader shader);
    std::optional<ComPtr<ID3D11PixelShader>> getLoadedPixelShader(Shader shader
    );

    bool loadBlobFromFilename(std::filesystem::directory_entry shaderFilepath);
    bool createShaders();

    private:
    D3D *graphicsManager;

    std::unordered_map<std::string, Shader> shaderFilenameToTypeMapping =
        {{"vertex", Shader::STANDARD_VERTEX}, {"pixel", Shader::LIGHTING_PIXEL}
    };
    std::unordered_map<Shader, ShaderType> shaderToShaderTypeMapping = {
        {Shader::STANDARD_VERTEX, ShaderType::VERTEX},
        {Shader::LIGHTING_PIXEL, ShaderType::PIXEL}
    };
    std::unordered_map<Shader, std::vector<D3D11_INPUT_ELEMENT_DESC>>
        shadeToInputElementDescriptionMapping = {
            {Shader::STANDARD_VERTEX,
             {{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
               D3D11_INPUT_PER_VERTEX_DATA, 0},
              {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
               D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
              {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
               D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}}}
    };
    std::unordered_map<Shader, ComPtr<ID3DBlob>> shaderToShaderBlobMapping;
    std::unordered_map<Shader, ComPtr<ID3D11VertexShader>>
        shaderToVertexShaderMapping;
    std::unordered_map<Shader, ComPtr<ID3D11PixelShader>>
        shaderToPixelShaderMapping;
};
