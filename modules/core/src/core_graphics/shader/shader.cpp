#include "shader.h"

#include <filesystem>
#include <spdlog/spdlog.h>

#include <core_utils/data.h>

bool BaseShader::loadBlob(D3D *graphicsManager)
{
    const std::filesystem::path shaderFilepathAbsolute =
        std::filesystem::absolute(std::filesystem::path(this->filepath));
    const std::string shaderFilepathString =
        shaderFilepathAbsolute.generic_string();

    spdlog::info("Loading shader: " + shaderFilepathString);
    this->blob = graphicsManager->loadShader(shaderFilepathString);
    if (!(this->blob))
        return false;

    return true;
}

bool BaseShader::load(D3D *graphicsManager)
{
    if (!this->loadBlob(graphicsManager))
        return false;
    if (!this->create(graphicsManager))
        return false;
    return true;
}

// TODO - repeated code
bool VertexShader::create(D3D *graphicsManager)
{
    this->shader = graphicsManager->createVertexShader(this->blob);
    this->inputLayout = graphicsManager->createInputLayout(
        this->inputElementDescription.value(), this->blob
    );
    return true;
}

bool VertexShader::set(D3D *graphicsManager)
{
    graphicsManager->deviceContext->VSSetShader(shader.Get(), nullptr, 0);
    return true;
}

bool DomainShader::create(D3D *graphicsManager)
{
    this->shader = graphicsManager->createDomainShader(this->blob);
    return true;
}

bool DomainShader::set(D3D *graphicsManager)
{
    graphicsManager->deviceContext->DSSetShader(shader.Get(), nullptr, 0);
    return true;
}

bool HullShader::create(D3D *graphicsManager)
{
    this->shader = graphicsManager->createHullShader(this->blob);
    return true;
}

bool HullShader::set(D3D *graphicsManager)
{
    graphicsManager->deviceContext->HSSetShader(shader.Get(), nullptr, 0);
    return true;
}

bool PixelShader::create(D3D *graphicsManager)
{
    this->shader = graphicsManager->createPixelShader(this->blob);
    return true;
}

bool PixelShader::set(D3D *graphicsManager)
{
    graphicsManager->deviceContext->PSSetShader(shader.Get(), nullptr, 0);
    return true;
}
