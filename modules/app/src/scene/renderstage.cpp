#include "renderstage.h"
#include "core_graphics/buffer/base.h"
#include <DirectXMath.h>

bool RenderStage::render(D3D *graphicsManager)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    BufferSlots slotCounters = BufferSlots();
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

    worldMatrix = XMMatrixIdentity();
    if (this->outputRenderTexture)
    {
        this->outputRenderTexture.value()->set();
        this->outputRenderTexture.value()->clear(this->clearColour.data());
        viewMatrix = this->camera->orthoViewMatrix;
        projectionMatrix =
            this->outputRenderTexture.value()->orthographicProjectionMatrix;
    }
    else
    {
        graphicsManager->setRenderTargetViewToBackBuffer();
        graphicsManager->setDefaultViewport();
        graphicsManager->clearBackBuffer(this->clearColour.data());
        viewMatrix = camera->orthoViewMatrix;
        projectionMatrix = graphicsManager->orthographicProjectionMatrix;
    }

    graphicsManager->setDepthStencilState(false);

    graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, this->mesh.value().vertexBuffer.GetAddressOf(), &stride, &offset
    );
    graphicsManager->deviceContext->IASetIndexBuffer(
        this->mesh.value().vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    graphicsManager->deviceContext->IASetInputLayout(
        this->shaderManagerState->standardVertexShader.inputLayout.value().Get()
    );

    this->shaderManagerState->matrixBuffer.setData(
        worldMatrix, viewMatrix, projectionMatrix
    );
    this->shaderManagerState->matrixBuffer.setShader(&slotCounters, 0);
    this->shaderManagerState->cameraBuffer.setShader(&slotCounters, 0);

    if (!this->inputRenderTexture->setPixelShader(&slotCounters))
        return false;
    if (!this->textureSampler->setPixelShader(graphicsManager, &slotCounters))
        return false;

    this->shaderManagerState->standardVertexShader.set(graphicsManager);
    this->shaderManagerState->textureOnlyPixelShader.set(graphicsManager);

    graphicsManager->deviceContext->DrawIndexed(
        this->mesh.value().getVertexIndexCount(), 0, 0
    );

    graphicsManager->setDepthStencilState(true);
    graphicsManager->setRenderTargetViewToBackBuffer();
    return true;
}
