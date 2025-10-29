#include "scene.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <wrl/client.h>

Scene::Scene(D3D *graphicsManager)
{
    this->graphicsManager = graphicsManager;
    this->pointLight = PointLight();
    this->pointLightBuffer = PointLightBuffer();
    this->pointLightBuffer.init(graphicsManager);
    this->pointLightBuffer.pointLight = &this->pointLight;
    this->pointLight.colour = {1.f, 1.f, 1.f, 1.f};
    this->pointLight.worldPosition = {0.f, 0.f, -10.f, 0.f};
    this->pointLight.direction = {0.f, 0.f, 1.f, 0.f};
}

bool Scene::drawMesh(
    UINT &stride, UINT &offset, BaseMesh *mesh, XMMATRIX meshWorldMatrix
)
{
    this->graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    this->graphicsManager->deviceContext->IASetIndexBuffer(
        mesh->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    this->graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );

    if (!this->graphicsManager->updateMatrixBuffer(
            meshWorldMatrix, this->camera->viewMatrix
        ))
        return false;

    if (!this->pointLightBuffer.setPixelShader())
        return false;

    this->graphicsManager->deviceContext->IASetInputLayout(
        this->graphicsManager->inputLayout.Get()
    );
    this->graphicsManager->deviceContext->VSSetShader(
        this->graphicsManager->vertexShader.Get(), nullptr, 0
    );
    this->graphicsManager->deviceContext->PSSetShader(
        this->graphicsManager->pixelShader.Get(), nullptr, 0
    );
    this->graphicsManager->deviceContext->DrawIndexed(
        mesh->getVertexIndexCount(), 0, 0
    );

    return true;
}

bool Scene::drawScreen()
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    this->graphicsManager->deviceContext->OMSetRenderTargets(
        1, this->graphicsManager->renderTargetView.GetAddressOf(),
        this->graphicsManager->depthStencilView.Get()
    );

    this->drawMesh(
        stride, offset, this->triangleMesh, this->triangleMeshWorldMatrix
    );
    this->drawMesh(
        stride, offset, this->sphereMesh, this->sphereMeshWorldMatrix
    );
    return true;
}

bool Scene::render()
{

    if (!this->drawScreen())
    {
        return false;
    }

    return true;
}

DebugScene::DebugScene(D3D *graphicsManager)
{
    this->graphicsManager = graphicsManager;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(this->graphicsManager->window->mainScale);
    style.FontScaleDpi = this->graphicsManager->window->mainScale;

    ImGui_ImplWin32_Init(this->graphicsManager->window->hwnd);
    ImGui_ImplDX11_Init(
        this->graphicsManager->device.Get(),
        this->graphicsManager->deviceContext.Get()
    );
}

DebugScene::~DebugScene()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool DebugScene::render()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Draggable Window");
    ImGui::Text("Hello from ImGui!");
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return true;
}
