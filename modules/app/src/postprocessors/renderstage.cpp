#include "renderstage.h"
#include "core_graphics/buffer/base.h"
#include <DirectXMath.h>

bool RenderStage::render(D3D *graphicsManager)
{
    BufferSlots slotCounters = BufferSlots();
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

    worldMatrix = XMMatrixIdentity();
    viewMatrix = this->camera->orthographicViewMatrix;

    graphicsManager->setRenderTargetViewToBackBuffer();
    graphicsManager->setDefaultViewport();

    if (this->outputRenderTexture)
    {
        this->outputRenderTexture.value()->set();
        this->outputRenderTexture.value()->clear(this->clearColour.data());
        projectionMatrix =
            this->outputRenderTexture.value()->orthographicProjectionMatrix;
    }
    else
    {
        graphicsManager->clearBackBuffer(this->clearColour.data());
        projectionMatrix = graphicsManager->orthographicProjectionMatrix;
    }

    graphicsManager->setDepthStencilState(false);

    this->mesh->sendMeshData(graphicsManager);

    graphicsManager->deviceContext->IASetInputLayout(
        this->shaderManagerState->standardVertexShader.inputLayout.value().Get()
    );

    this->shaderManagerState->matrixBuffer.setData(
        worldMatrix, viewMatrix, projectionMatrix
    );
    if (!this->shaderManagerState->matrixBuffer.setShader(&slotCounters, 0))
        return false;
    if (!this->shaderManagerState->cameraVertexBuffer.setShader(
            &slotCounters, 0
        ))
        return false;

    if (!this->inputRenderTexture->setPixelShader(&slotCounters))
        return false;
    if (!this->textureSampler->setPixelShader(graphicsManager, &slotCounters))
        return false;

    for (int bufferIndex = 0;
         bufferIndex < this->pixelShader->bufferStack.size(); bufferIndex++)
    {
        this->pixelShader->bufferStack[bufferIndex]->setShader(
            &slotCounters, 1
        );
    }

    this->shaderManagerState->standardVertexShader.set(graphicsManager);
    this->pixelShader->set(graphicsManager);

    graphicsManager->deviceContext->DrawIndexed(
        this->mesh.value().getVertexIndexCount(), 0, 0
    );

    graphicsManager->setDepthStencilState(true);
    return true;
}
