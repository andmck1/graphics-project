#include "scene.h"
#include "renderstage.h"
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
//
// bool Scene::updateAndDraw()
// {
//     //
//     this->state->graphicsManager->clearScreen(this->state->clearColour.data());
//     this->state->graphicsManager->clearBackBuffer(
//         this->state->clearColour.data()
//         // this->state->graphicsManager->renderTargetView,
//         // this->state->graphicsManager->depthStencilView
//     );
//     this->state->graphicsManager->deviceContext->OMSetRenderTargets(
//         1, this->state->graphicsManager->renderTargetView.GetAddressOf(),
//         this->state->graphicsManager->depthStencilView.Get()
//     );
//
//     for (int i = 0; i < this->state->actors.size(); i++)
//     {
//         SceneActor *actor = this->state->actors[i].get();
//         if (actor->isUpdateEnabled)
//         {
//             actor->moveTo(
//                 actor->worldPosition[0], actor->worldPosition[1],
//                 actor->worldPosition[2]
//             );
//             actor->update();
//         }
//         if (actor->isDrawEnabled)
//         {
//             {
//                 this->state->shaderManagerState->matrixBuffer.setData(
//                     actor->getWorldMatrix(), this->state->camera->viewMatrix,
//                     this->state->graphicsManager->projectionMatrix
//                 );
//
//                 if (!actor->drawMesh(this->state->graphicsManager))
//                     return false;
//             }
//         }
//     }
//     return true;
// }
//
bool Scene::updateAndDraw()
{
    if (this->state->renderStages.size() > 0)
    {
        RenderStage *initialRenderStage = this->state->renderStages[0].get();
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

    if (this->state->renderStages.size() > 0)
    {
        for (const std::unique_ptr<RenderStage> &renderStagePtr :
             this->state->renderStages)
        {
            renderStagePtr->render(this->state->graphicsManager);
        }
    }

    return true;
}
