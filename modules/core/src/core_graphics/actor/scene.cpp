#include "scene.h"
#include "core_graphics/buffer/base.h"
#include "core_graphics/d3d/shadowmap.h"
#include "core_graphics/shader/shader.h"

SceneActor::SceneActor()
{
    this->worldPosition = {0.f, 0.f, 0.f};
    this->scaling = {1.f, 1.f, 1.f};
    this->rotation = {0.f, 0.f, 0.f};
    this->update();
}

void SceneActor::update() {};

bool SceneActor::drawMesh(D3D *graphicsManager, bool isTextureSet)
{
    BufferSlots slotCounters = BufferSlots();

    graphicsManager->deviceContext->HSSetShader(nullptr, nullptr, 0);
    graphicsManager->deviceContext->DSSetShader(nullptr, nullptr, 0);

    this->mesh->get()->sendMeshData(graphicsManager);

    if (isTextureSet)
    {
        if (!this->texture->setPixelShader(&slotCounters, 1))
            return false;

        if (!this->textureSampler->setPixelShader(
                graphicsManager, &slotCounters
            ))
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

        for (ShadowMap *shadowMap : this->shadowMaps)
        {
            if (!shadowMap->setPixelShader(&slotCounters))
                return false;
        }
    }

    BufferSlots loopSlotCounters = slotCounters;
    for (int shaderIndex = 0;
         shaderIndex < this->currentShaderStack->shaders.size(); shaderIndex++)
    {
        BaseShader *shader = this->currentShaderStack->shaders[shaderIndex];
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
        this->mesh->get()->getVertexIndexCount(), 0, 0
    );

    graphicsManager->deviceContext->HSSetShader(nullptr, nullptr, 0);
    graphicsManager->deviceContext->DSSetShader(nullptr, nullptr, 0);
    return true;
}
