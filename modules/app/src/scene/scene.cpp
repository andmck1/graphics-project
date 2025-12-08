#include "scene.h"
#include "../postprocessors/renderstage.h"

#include "core_graphics/actor/scene.h"
#include "core_graphics/light/spotlight.h"
#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <wrl/client.h>

Scene::Scene(SceneState *state)
{
    this->state = state;
}

bool Scene::updateAndDraw()
{
    this->state->graphicsManager->setWireframeEnabled(
        this->state->isWireframeEnabled
    );

    // Depth pass
    for (int spotLightIndex = 0;
         spotLightIndex < this->state->spotLights.size(); spotLightIndex++)
    {
        SpotLight *spotLightPtr = this->state->spotLights[spotLightIndex];

        spotLightPtr->shadowMap.set();
        XMMATRIX lightViewMatrix = spotLightPtr->calculateViewMatrix();
        XMMATRIX lightProjectionMatrix =
            spotLightPtr->calculateProjectionMatrix(1000.f, 50000.f);
        for (int i = 0; i < this->state->actors.size(); i++)
        {
            SceneActor *actor = this->state->actors[i].get();
            if ((actor->isDrawEnabled) && (actor->depthShaderStack != nullptr))
            {
                actor->currentShaderStack = actor->depthShaderStack;
                this->state->shaderManagerState->matrixVertexBuffer.setData(
                    actor->getWorldMatrix(), lightViewMatrix,
                    lightProjectionMatrix
                );
                this->state->shaderManagerState->matrixDomainBuffer.setData(
                    actor->getWorldMatrix(), lightViewMatrix,
                    lightProjectionMatrix
                );
                if (!actor->drawMesh(this->state->graphicsManager, false))
                    return false;
            }
        }
        this->state->graphicsManager->setRenderTargetViewToBackBuffer();
        this->state->graphicsManager->setDefaultViewport();
    }

    for (int directionalLightIndex = 0;
         directionalLightIndex < this->state->directionalLights.size();
         directionalLightIndex++)
    {
        DirectionalLight *directionalLightPtr =
            this->state->directionalLights[directionalLightIndex];

        directionalLightPtr->shadowMap.set();
        XMMATRIX lightViewMatrix = directionalLightPtr->calculateViewMatrix();
        XMMATRIX lightProjectionMatrix =
            directionalLightPtr->calculateOrthographicProjectionMatrix(
                100000, 100000, .1f, 50000.f
            );
        for (int i = 0; i < this->state->actors.size(); i++)
        {
            SceneActor *actor = this->state->actors[i].get();
            if ((actor->isDrawEnabled) && (actor->depthShaderStack != nullptr))
            {
                actor->currentShaderStack = actor->depthShaderStack;
                this->state->shaderManagerState->matrixVertexBuffer.setData(
                    actor->getWorldMatrix(), lightViewMatrix,
                    lightProjectionMatrix
                );
                this->state->shaderManagerState->matrixDomainBuffer.setData(
                    actor->getWorldMatrix(), lightViewMatrix,
                    lightProjectionMatrix
                );
                if (!actor->drawMesh(this->state->graphicsManager, false))
                    return false;
            }
        }
        this->state->graphicsManager->setRenderTargetViewToBackBuffer();
        this->state->graphicsManager->setDefaultViewport();
    }

    // Set to render to texture if needed
    if ((this->state->crtPostProcessor.renderStages.size() > 0) &&
        (this->state->crtPostProcessor.isEnabled))
    {
        RenderStage *initialRenderStage =
            this->state->crtPostProcessor.renderStages[0].get();
        initialRenderStage->inputRenderTexture->set();
        initialRenderStage->inputRenderTexture->clear(
            this->state->clearColour.data()
        );
    }
    else
    {
        this->state->graphicsManager->clearBackBuffer(
            this->state->clearColour.data()
        );
        this->state->graphicsManager->setRenderTargetViewToBackBuffer();
        this->state->graphicsManager->setDefaultViewport();
    }

    // Draw scene
    this->state->graphicsManager->setDepthStencilState(true);
    for (int i = 0; i < this->state->actors.size(); i++)
    {
        SceneActor *actor = this->state->actors[i].get();
        if (actor->isUpdateEnabled)
        {
            actor->moveTo(
                actor->worldPosition[0], actor->worldPosition[1],
                actor->worldPosition[2]
            );
            actor->update();
        }
        if (actor->isDrawEnabled)
        {
            XMMATRIX spotLightViewMatrix0 =
                this->state->spotLights[0]->calculateViewMatrix();
            XMMATRIX spotLightProjectionMatrix0 =
                this->state->spotLights[0]->calculateProjectionMatrix(
                    1000.f, 50000.f
                );
            XMMATRIX spotLightViewMatrix1 =
                this->state->spotLights[1]->calculateViewMatrix();
            XMMATRIX spotLightProjectionMatrix1 =
                this->state->spotLights[1]->calculateProjectionMatrix(
                    1000.f, 50000.f
                );
            XMMATRIX directionalLightViewMatrix0 =
                this->state->directionalLights[0]->calculateViewMatrix();
            XMMATRIX directionalLightProjectionMatrix0 =
                this->state->directionalLights[0]
                    ->calculateOrthographicProjectionMatrix(
                        100000, 100000, 0.1, 50000.f
                    );
            XMMATRIX directionalLightViewMatrix1 =
                this->state->directionalLights[1]->calculateViewMatrix();
            XMMATRIX directionalLightProjectionMatrix1 =
                this->state->directionalLights[1]
                    ->calculateOrthographicProjectionMatrix(
                        100000, 100000, .1, 50000.f
                    );
            actor->currentShaderStack = actor->defaultShaderStack;
            this->state->shaderManagerState->matrixVertexBuffer.setData(
                actor->getWorldMatrix(), this->state->camera->viewMatrix,
                this->state->graphicsManager->projectionMatrix,
                spotLightViewMatrix0, spotLightProjectionMatrix0,
                spotLightViewMatrix1, spotLightProjectionMatrix1,
                directionalLightViewMatrix0, directionalLightProjectionMatrix0,
                directionalLightViewMatrix1, directionalLightProjectionMatrix1

            );
            this->state->shaderManagerState->matrixHullBuffer.setData(
                actor->getWorldMatrix(), this->state->camera->viewMatrix,
                this->state->graphicsManager->projectionMatrix
            );
            this->state->shaderManagerState->matrixDomainBuffer.setData(
                actor->getWorldMatrix(), this->state->camera->viewMatrix,
                this->state->graphicsManager->projectionMatrix,
                spotLightViewMatrix0, spotLightProjectionMatrix0,
                spotLightViewMatrix1, spotLightProjectionMatrix1,
                directionalLightViewMatrix0, directionalLightProjectionMatrix0,
                directionalLightViewMatrix1, directionalLightProjectionMatrix1
            );
            if (!actor->drawMesh(this->state->graphicsManager))
                return false;
        }
    }

    this->state->graphicsManager->setRenderTargetViewToBackBuffer();

    // Post processing
    if ((this->state->crtPostProcessor.renderStages.size() > 0) &&
        (this->state->crtPostProcessor.isEnabled))
    {
        for (const std::unique_ptr<RenderStage> &renderStagePtr :
             this->state->crtPostProcessor.renderStages)
        {
            if (!renderStagePtr->render(this->state->graphicsManager))
                return false;
        }
    }

    return true;
}
