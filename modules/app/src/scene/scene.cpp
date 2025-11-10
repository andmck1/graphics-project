#include "scene.h"
#include <spdlog/spdlog.h>
#include <d3d11.h>
#include <wrl/client.h>

Scene::Scene(SceneState *state)
{
    this->state = state;
}

bool Scene::firstPass()
{
    this->state->graphicsManager->clearScreen(this->state->clearColour.data());
    // this->state->graphicsManager->deviceContext->OMSetRenderTargets(
    //     1, this->state->graphicsManager->renderTargetView.GetAddressOf(),
    //     this->state->graphicsManager->depthStencilView.Get()
    // );
    return true;
}

bool Scene::finalPass()
{
    return true;
}

bool Scene::updateAndDraw()
{
    this->state->graphicsManager->clearScreen(this->state->clearColour.data());

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
    return true;
}
