#include "scene.h"

#include <memory>
#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <wrl/client.h>

Scene::Scene(SceneConfig *config)
{
    this->graphicsManager = config->graphicsManager;
    this->shaderManager = config->shaderManager;

    this->pointLight = PointLight();
    this->pointLightBuffer = PointLightBuffer();
    this->pointLightBuffer.init(graphicsManager);
    this->pointLightBuffer.pointLight = &this->pointLight;
    this->pointLight.colour = {1.f, 1.f, 1.f, 1.f};
    this->pointLight.worldPosition = {0.f, 0.f, -10.f, 0.f};
    this->pointLight.direction = {0.f, 0.f, 1.f, 0.f};

    this->camera = &config->camera;
    this->meshes = &config->meshes;
}

bool Scene::drawMesh(UINT &stride, UINT &offset, SceneMesh *sceneMesh)
{
    this->graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, sceneMesh->mesh->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    this->graphicsManager->deviceContext->IASetIndexBuffer(
        sceneMesh->mesh->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    this->graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    if (!this->graphicsManager->updateMatrixBuffer(
            sceneMesh->worldMatrix, this->camera->viewMatrix
        ))
        return false;

    if (!this->pointLightBuffer.setPixelShader())
        return false;

    for (std::unique_ptr<BaseShader> &shaderPtr :
         *sceneMesh->shaderStack->shaders)
    {
        if (shaderPtr->inputElementDescription)
        {
            ComPtr<ID3D11InputLayout> inputLayout =
                this->graphicsManager->createInputLayout(
                    shaderPtr->inputElementDescription.value(), shaderPtr->blob
                );
            this->graphicsManager->deviceContext->IASetInputLayout(
                inputLayout.Get()
            );
        }

        // set shader
        switch (shaderPtr->type)
        {
        case ShaderType::VERTEX:
        {
            VertexShader *vertexShader =
                dynamic_cast<VertexShader *>(shaderPtr.get());
            if (!shaderPtr)
            {
                spdlog::error("Vertex shader not loaded.");
                return false;
            }
            this->graphicsManager->deviceContext->VSSetShader(
                vertexShader->shader.Get(), nullptr, 0
            );
            break;
        }
        case ShaderType::PIXEL:
        {
            PixelShader *pixelShader =
                dynamic_cast<PixelShader *>(shaderPtr.get());
            if (!pixelShader)
            {
                spdlog::error("Pixel shader not loaded.");
                return false;
            }
            this->graphicsManager->deviceContext->PSSetShader(
                pixelShader->shader.Get(), nullptr, 0
            );
            break;
        }
        default:
        {
            spdlog::error("Shader type not implemented.");
            return false;
        }
        }
    }

    this->graphicsManager->deviceContext->DrawIndexed(
        sceneMesh->mesh->getVertexIndexCount(), 0, 0
    );

    return true;
}

bool Scene::drawScreen()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->graphicsManager->deviceContext->OMSetRenderTargets(
        1, this->graphicsManager->renderTargetView.GetAddressOf(),
        this->graphicsManager->depthStencilView.Get()
    );

    for (std::unique_ptr<SceneMesh> &sceneMeshPtr : *this->meshes)
    {
        this->drawMesh(stride, offset, sceneMeshPtr.get());
    }

    return true;
}

bool Scene::render()
{

    if (!this->drawScreen())
    {
        return false;
    }

    return true;
}
