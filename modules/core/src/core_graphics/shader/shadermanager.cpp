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
    for (BaseShader *shader : this->config->shaders)
    {
        if (!shader->loadBlob(this->config->graphicsManager))
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
    for (BaseShader *shader : this->config->shaders)
    {
        switch (shader->type)
        {
        case (ShaderType::VERTEX):
        {
            VertexShader *vertexShader = static_cast<VertexShader *>(shader);
            vertexShader->shader =
                this->config->graphicsManager->createVertexShader(shader->blob);
            break;
        }
        case (ShaderType::PIXEL):
        {
            PixelShader *pixelShader = static_cast<PixelShader *>(shader);
            pixelShader->shader =
                this->config->graphicsManager->createPixelShader(shader->blob);
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
