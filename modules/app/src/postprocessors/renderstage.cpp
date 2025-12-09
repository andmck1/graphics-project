#include "renderstage.h"
#include "core_graphics/buffer/base.h"
#include <DirectXMath.h>

bool RenderStage::render(D3D *graphicsManager)
{
    BufferSlots slotCounters = BufferSlots();
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

    worldMatrix = XMMatrixIdentity();
    viewMatrix = this->camera->orthographicViewMatrix;
    
    // reset back buffer and viewport
    graphicsManager->setRenderTargetViewToBackBuffer();
    graphicsManager->setDefaultViewport();
    
    // if outputting to render texture then set, otherwise set to back buffer
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
    
    // turn off depth
    graphicsManager->setDepthStencilState(false);
    
    // send mesh (set vertex and index buffers)
    this->mesh->sendMeshData(graphicsManager);
    
    // set input layout for standard vertex shader
    graphicsManager->deviceContext->IASetInputLayout(
        this->shaderManagerState->standardVertexShader.inputLayout.value().Get()
    );
    
    // set shader data
    this->shaderManagerState->matrixVertexBuffer.setData(
        worldMatrix, viewMatrix, projectionMatrix
    );
    if (!this->shaderManagerState->matrixVertexBuffer.setShader(&slotCounters, 0))
        return false;
    if (!this->shaderManagerState->cameraVertexBuffer.setShader(
            &slotCounters, 0
        ))
        return false;
    
    // set shaders for input render texture and sampler
    if (!this->inputRenderTexture->setPixelShader(&slotCounters))
        return false;
    if (!this->textureSampler->setPixelShader(graphicsManager, &slotCounters))
        return false;

    // set shader data for buffer stack
    for (int bufferIndex = 0;
         bufferIndex < this->pixelShader->bufferStack.size(); bufferIndex++)
    {
        this->pixelShader->bufferStack[bufferIndex]->setShader(
            &slotCounters, 1
        );
    }
    
    // set shaders
    this->shaderManagerState->standardVertexShader.set(graphicsManager);
    this->pixelShader->set(graphicsManager);
    
    // draw
    graphicsManager->deviceContext->DrawIndexed(
        this->mesh.value().getVertexIndexCount(), 0, 0
    );
    
    // turn on depth
    graphicsManager->setDepthStencilState(true);
    return true;
}
