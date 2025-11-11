#include "scene.h"
#include "../postprocessors/renderstage.h"
#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <wrl/client.h>

Scene::Scene(SceneState *state)
{
    this->state = state;
}

#include "scene.h"

#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <wrl/client.h>

bool Scene::updateAndDraw()
{
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
    }
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
            {
                this->state->shaderManagerState->matrixBuffer.setData(
                    actor->getWorldMatrix(), this->state->camera->viewMatrix,
                    this->state->graphicsManager->projectionMatrix
                );

                if (!actor->drawMesh(this->state->graphicsManager))
                    return false;
            }
        }
    }

    this->state->graphicsManager->setRenderTargetViewToBackBuffer();

    if ((this->state->crtPostProcessor.renderStages.size() > 0) &&
        (this->state->crtPostProcessor.isEnabled))
    {
        for (const std::unique_ptr<RenderStage> &renderStagePtr :
             this->state->crtPostProcessor.renderStages)
        {
            if(!renderStagePtr->render(this->state->graphicsManager))
                return false;
        }
    }

    return true;
}
