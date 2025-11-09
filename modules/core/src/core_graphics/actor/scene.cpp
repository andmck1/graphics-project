#include "scene.h"
#include "core_graphics/buffer/base.h"
#include "core_graphics/shader/shader.h"

SceneActor::SceneActor()
{
    this->worldPosition = {0.f, 0.f, 0.f};
    this->scaling = {1.f, 1.f, 1.f};
    this->rotation = {0.f, 0.f, 0.f};
    this->update();
}

void SceneActor::update()
{
}

bool SceneActor::drawMesh(D3D *graphicsManager)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    BufferSlots slotCounters = BufferSlots();

    graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, this->mesh->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    graphicsManager->deviceContext->IASetIndexBuffer(
        this->mesh->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    if (!this->texture->setPixelShader(&slotCounters, 1))
        return false;

    if (!this->textureSampler->setPixelShader(graphicsManager, &slotCounters))
        return false;

    if (this->heightMap != nullptr)
    {
        if (!this->heightMapSampler->setVertexShader(
                graphicsManager, &slotCounters
            ))
            return false;
        if (!this->heightMap->setVertexShader(&slotCounters, 1))
            return false;
    }

    BufferSlots loopSlotCounters = slotCounters;
    for (int shaderIndex = 0; shaderIndex < this->shaderStack->shaders.size();
         shaderIndex++)
    {
        BaseShader *shader = this->shaderStack->shaders[shaderIndex];
        if (shader->inputLayout)
        {
            graphicsManager->deviceContext->IASetInputLayout(
                shader->inputLayout.value().Get()
            );
        }

        for (int bufferIndex = 0; bufferIndex < shader->bufferStack.size();
             bufferIndex++)
        {
            BaseBuffer *buffer = shader->bufferStack[bufferIndex];
            buffer->setShader(&loopSlotCounters, 1);
        }

        shader->set(graphicsManager);
        loopSlotCounters = slotCounters;
    }

    graphicsManager->deviceContext->DrawIndexed(
        this->mesh->getVertexIndexCount(), 0, 0
    );

    return true;
}
