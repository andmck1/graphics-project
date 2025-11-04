#include <memory>
#include <spdlog/spdlog.h>

#include "shadermanager.h"

ShaderManager::ShaderManager(ShaderManagerConfig *config)
{
    this->config = config;
    this->loadShaders();
    this->createShaders();
}

bool ShaderManager::loadShaders()
{
    for (std::unique_ptr<BaseShader> &shaderPtr : this->config->shaders)
    {
        if (!shaderPtr->loadBlob(this->config->graphicsManager))
        {
            spdlog::error("Failed to load shader.");
            return false;
        }
    }
    return true;
}

bool ShaderManager::createShaders()
{
    spdlog::info("Creating shaders");
    for (std::unique_ptr<BaseShader> &shaderPtr : this->config->shaders)
    {
        switch (shaderPtr->type)
        {
        case (ShaderType::VERTEX):
        {
            VertexShader *vertexShader =
                dynamic_cast<VertexShader *>(shaderPtr.get());
            vertexShader->shader =
                this->config->graphicsManager->createVertexShader(
                    shaderPtr->blob
                );
            break;
        }
        case (ShaderType::PIXEL):
        {
            PixelShader *pixelShader =
                dynamic_cast<PixelShader *>(shaderPtr.get());
            pixelShader->shader =
                this->config->graphicsManager->createPixelShader(
                    shaderPtr->blob
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
