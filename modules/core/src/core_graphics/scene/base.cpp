#include "base.h"

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>
#include <wrl/client.h>

bool BaseScene::drawScreen()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->graphicsManager->deviceContext->OMSetRenderTargets(
        1, this->graphicsManager->renderTargetView.GetAddressOf(),
        this->graphicsManager->depthStencilView.Get()
    );

    for (BaseSceneMesh sceneMesh : this->meshes)
    {
        this->drawMesh(stride, offset, &sceneMesh);
    }

    return true;
}

bool BaseScene::render()
{

    if (!this->drawScreen())
    {
        return false;
    }

    return true;
}
