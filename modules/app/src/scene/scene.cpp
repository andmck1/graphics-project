#include "scene.h"

#include <memory>
#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <wrl/client.h>

Scene::Scene(SceneState *state)
{
    this->state = state;
}

bool Scene::drawMesh(UINT &stride, UINT &offset, SceneMesh *sceneMesh)
{
    this->state->graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, sceneMesh->mesh->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    this->state->graphicsManager->deviceContext->IASetIndexBuffer(
        sceneMesh->mesh->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    this->state->graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    if (!this->state->graphicsManager->updateMatrixBuffer(
            sceneMesh->worldMatrix, this->state->camera.viewMatrix, 0, 1
        ))
        return false;

    if (!this->state->cameraBuffer->setVertexShader(1, 1))
        return false;

    if (!this->state->ambientLightBuffer->setPixelShader(0, 1))
        return false;

    if (!this->state->pointLightsBuffer->setPixelShader(1, 1))
        return false;

    if (!sceneMesh->texture->setPixelShader(0, 1))
        return false;

    if (!sceneMesh->sampler->setPixelShader(this->state->graphicsManager))
        return false;

    for (std::unique_ptr<BaseShader> &shaderPtr :
         *sceneMesh->shaderStack->shaders)
    {
        if (shaderPtr->inputElementDescription)
        {
            ComPtr<ID3D11InputLayout> inputLayout =
                this->state->graphicsManager->createInputLayout(
                    shaderPtr->inputElementDescription.value(), shaderPtr->blob
                );
            this->state->graphicsManager->deviceContext->IASetInputLayout(
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
            this->state->graphicsManager->deviceContext->VSSetShader(
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
            this->state->graphicsManager->deviceContext->PSSetShader(
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

    this->state->graphicsManager->deviceContext->DrawIndexed(
        sceneMesh->mesh->getVertexIndexCount(), 0, 0
    );

    return true;
}

bool Scene::drawScreen()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->state->graphicsManager->deviceContext->OMSetRenderTargets(
        1, this->state->graphicsManager->renderTargetView.GetAddressOf(),
        this->state->graphicsManager->depthStencilView.Get()
    );

    for (std::unique_ptr<SceneMesh> &sceneMesh : this->state->meshes)
    {
        this->drawMesh(stride, offset, sceneMesh.get());
    }

    return true;
}

bool Scene::render()
{
    this->state->graphicsManager->clearScreen(this->state->clearColour.data());
    if (!this->drawScreen())
    {
        return false;
    }

    return true;
}
