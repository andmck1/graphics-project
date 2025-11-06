#include <spdlog/spdlog.h>

#include "base.h"

BaseShaderManager::BaseShaderManager(BaseShaderManagerState *state)
{
    this->state = state;
    this->loadShaders();
    this->createShaders();
}

bool BaseShaderManager::loadShaders()
{
    for (std::unique_ptr<BaseShader> &shaderPtr : this->state->shaders)
    {
        if (!shaderPtr->loadBlob(this->state->graphicsManager))
        {
            spdlog::error("Failed to load shader.");
            return false;
        }
    }
    return true;
}

bool BaseShaderManager::createShaders()
{
    spdlog::info("Creating shaders");
    for (std::unique_ptr<BaseShader> &shaderPtr : this->state->shaders)
    {
        switch (shaderPtr->type)
        {
        case (ShaderType::VERTEX):
        {
            VertexShader *vertexShader =
                dynamic_cast<VertexShader *>(shaderPtr.get());
            vertexShader->shader =
                this->state->graphicsManager->createVertexShader(
                    shaderPtr->blob
                );
            break;
        }
        case (ShaderType::PIXEL):
        {
            PixelShader *pixelShader =
                dynamic_cast<PixelShader *>(shaderPtr.get());
            pixelShader->shader =
                this->state->graphicsManager->createPixelShader(
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
