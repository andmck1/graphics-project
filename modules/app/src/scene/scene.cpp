#include "scene.h"
#include "core_graphics/light/point.h"

#include <memory>
#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <wrl/client.h>

Scene::Scene(SceneState *state)
{
    this->state = state;
}

bool Scene::drawMesh(UINT &stride, UINT &offset, SceneActor *sceneActor)
{
    this->state->graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, sceneActor->mesh->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    this->state->graphicsManager->deviceContext->IASetIndexBuffer(
        sceneActor->mesh->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    this->state->graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    if (!this->state->graphicsManager->updateMatrixBuffer(
            sceneActor->getWorldMatrix(), this->state->camera.viewMatrix, 0, 1
        ))
        return false;

    if (!this->state->cameraBuffer->setVertexShader(1, 1))
        return false;

    if (sceneActor->heightMap != nullptr)
    {
        if (!sceneActor->heightMapSampler->setVertexShader(
                this->state->graphicsManager
            ))
            return false;
        if (!sceneActor->heightMap->setVertexShader(0, 1))
            return false;
    }

    // TODO - GET RID OF THIS CHECK
    if (sceneActor->isWave)
    {
        if (!this->state->waveDataBuffer->setVertexShader(2, 1))
            return false;
    }

    if (!this->state->ambientLightBuffer->setPixelShader(0, 1))
        return false;

    if (!this->state->pointLightsBuffer->setPixelShader(1, 1))
        return false;

    if (!this->state->directionalLightsBuffer->setPixelShader(2, 1))
        return false;

    if (!sceneActor->texture->setPixelShader(0, 1))
        return false;

    if (!sceneActor->textureSampler->setPixelShader(
            this->state->graphicsManager
        ))
        return false;

    for (BaseShader *shaderPtr : sceneActor->shaderStack->shaders)
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
                dynamic_cast<VertexShader *>(shaderPtr);
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
            PixelShader *pixelShader = dynamic_cast<PixelShader *>(shaderPtr);
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
        sceneActor->mesh->getVertexIndexCount(), 0, 0
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

    for (std::unique_ptr<SceneActor> &sceneActor : this->state->actors)
    {
        this->drawMesh(stride, offset, sceneActor.get());
    }

    return true;
}

void Scene::updateMeshPositions()
{
    for (int i = 0; i < this->state->pointLights.size(); i++)
    {
        PointLight *pointLight = this->state->pointLights[i].get();
        pointLight->moveTo(
            pointLight->worldPosition[0], pointLight->worldPosition[1],
            pointLight->worldPosition[2]
        );
    }
}

bool Scene::render()
{
    this->state->graphicsManager->clearScreen(this->state->clearColour.data());
    this->updateMeshPositions();
    if (!this->drawScreen())
    {
        return false;
    }

    return true;
}
