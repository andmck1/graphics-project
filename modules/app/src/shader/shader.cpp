#include "shader.h"

#include "spdlog/spdlog.h"

#include "utils/data.h"

ShaderManager::ShaderManager(D3D *graphicsManager)
{
    this->graphicsManager = graphicsManager;
}

bool ShaderManager::createShaders()
{
    spdlog::info("Creating shaders");
    for (int i = 0; i != static_cast<int>(Shader::END); i++)
    {
        Shader shader = static_cast<Shader>(i);
        std::optional<ShaderType> shaderType = this->getShaderType(shader);
        if (!shaderType)
        {
            spdlog::error("Shader type mapping not implemented.");
            return false;
        }
        std::optional<ComPtr<ID3DBlob>> shaderBlob =
            this->getShaderBlob(shader);
        if (!shaderBlob)
        {
            spdlog::error("Shader blob mapping not implemented.");
            return false;
        }

        switch (shaderType.value())
        {
        case (ShaderType::VERTEX):
        {
            this->shaderToVertexShaderMapping.insert(
                {shader,
                 this->graphicsManager->createVertexShader(shaderBlob.value())}
            );
            break;
        }
        case (ShaderType::PIXEL):
        {
            this->shaderToPixelShaderMapping.insert(
                {shader,
                 this->graphicsManager->createPixelShader(shaderBlob.value())}
            );
            break;
        }
        default:
        {
            spdlog::error("Shader type not implemented, cannot create shader.");
            return false;
        }
        }
    }
    return true;
}

std::optional<ShaderManager::Shader>
ShaderManager::getShaderFromFilename(std::string filename)
{
    std::optional<Shader> shader =
        getValueFromUnorderedMap(this->shaderFilenameToTypeMapping, filename);
    return shader;
}

std::optional<ShaderManager::ShaderType>
ShaderManager::getShaderType(ShaderManager::Shader shader)
{
    std::optional<ShaderType> shaderType =
        getValueFromUnorderedMap(this->shaderToShaderTypeMapping, shader);
    return shaderType;
}

std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
ShaderManager::getInputElementDescription(ShaderManager::Shader shader)
{
    std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
        inputElementDescriptions = getValueFromUnorderedMap(
            this->shadeToInputElementDescriptionMapping, shader
        );
    return inputElementDescriptions;
}

std::optional<ComPtr<ID3DBlob>>
ShaderManager::getShaderBlob(ShaderManager::Shader shader)
{
    std::optional<ComPtr<ID3DBlob>> shaderBlob =
        getValueFromUnorderedMap(this->shaderToShaderBlobMapping, shader);
    return shaderBlob;
}

std::optional<ComPtr<ID3D11VertexShader>>
ShaderManager::getLoadedVertexShader(ShaderManager::Shader shader)
{
    std::optional<ComPtr<ID3D11VertexShader>> vertexShader =
        getValueFromUnorderedMap(this->shaderToVertexShaderMapping, shader);
    return vertexShader;
}

std::optional<ComPtr<ID3D11PixelShader>>
ShaderManager::getLoadedPixelShader(ShaderManager::Shader shader)
{
    std::optional<ComPtr<ID3D11PixelShader>> pixelShader =
        getValueFromUnorderedMap(this->shaderToPixelShaderMapping, shader);
    return pixelShader;
}

bool ShaderManager::loadBlobFromFilename(
    std::filesystem::directory_entry shaderFilepath
)
{
    const std::filesystem::path shaderFilepathAbsolute =
        std::filesystem::absolute(shaderFilepath);
    const std::string shaderFilepathString =
        shaderFilepathAbsolute.generic_string();

    spdlog::info("Loading shader: " + shaderFilepathString);
    ComPtr<ID3DBlob> shaderBlob =
        this->graphicsManager->loadShader(shaderFilepathString);
    if (!shaderBlob)
        return false;

    std::optional<ShaderManager::Shader> shader =
        this->getShaderFromFilename(shaderFilepath.path().stem().string());
    if (!shader)
        return false;

    this->shaderToShaderBlobMapping.insert({shader.value(), shaderBlob});
    return true;
}
