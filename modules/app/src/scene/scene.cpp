#include "scene.h"

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>
#include <filesystem>
#include <wrl/client.h>

Scene::Scene(SceneConfig *config)
{
    this->graphicsManager = config->graphicsManager;
    this->shaderManager = config->shaderManager;
    this->loadShadersFromDirectory(config->vertexShaderDirectory);
    this->loadShadersFromDirectory(config->pixelShaderDirectory);
    this->shaderManager->createShaders();

    this->pointLight = PointLight();
    this->pointLightBuffer = PointLightBuffer();
    this->pointLightBuffer.init(graphicsManager);
    this->pointLightBuffer.pointLight = &this->pointLight;
    this->pointLight.colour = {1.f, 1.f, 1.f, 1.f};
    this->pointLight.worldPosition = {0.f, 0.f, -10.f, 0.f};
    this->pointLight.direction = {0.f, 0.f, 1.f, 0.f};
}

bool Scene::loadShadersFromDirectory(std::string shaderDirectory)
{
    for (const auto &shaderFilepath :
         std::filesystem::directory_iterator(shaderDirectory))
    {
        if (shaderFilepath.path().extension() == ".cso")
        {
            if (!this->shaderManager->loadBlobFromFilename(shaderFilepath))
            {
                spdlog::error("Failed to load shader.");
                return false;
            }
        }
    }
    return true;
}

bool Scene::drawMesh(
    UINT &stride, UINT &offset, BaseMesh *mesh, XMMATRIX meshWorldMatrix,
    std::vector<ShaderManager::Shader> shaderStack
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

    for (ShaderManager::Shader shader : shaderStack)
    {
        std::optional<ShaderManager::ShaderType> shaderType =
            this->shaderManager->getShaderType(shader);
        if (!shaderType)
        {
            spdlog::error("Shader to shader type mapping not implemented.");
            return false;
        }

        std::optional<ComPtr<ID3DBlob>> shaderBlob =
            this->shaderManager->getShaderBlob(shader);
        if (!shaderBlob)
        {
            spdlog::error("Shader to shader blob mapping doesn't exist.");
            return false;
        }

        std::optional<std::vector<D3D11_INPUT_ELEMENT_DESC>>
            inputElementDescription =
                this->shaderManager->getInputElementDescription(shader);

        if (inputElementDescription)
        {
            ComPtr<ID3D11InputLayout> inputLayout =
                this->graphicsManager->createInputLayout(
                    inputElementDescription.value(), shaderBlob.value()
                );
            this->graphicsManager->deviceContext->IASetInputLayout(
                inputLayout.Get()
            );
        }

        // set shader
        switch (shaderType.value())
        {
        case ShaderManager::ShaderType::VERTEX:
        {
            std::optional<ComPtr<ID3D11VertexShader>> vertexShader =
                this->shaderManager->getLoadedVertexShader(shader);
            if (!vertexShader)
            {
                spdlog::error("Vertex shader not loaded.");
                return false;
            }
            this->graphicsManager->deviceContext->VSSetShader(
                vertexShader.value().Get(), nullptr, 0
            );
            break;
        }
        case ShaderManager::ShaderType::PIXEL:
        {
            std::optional<ComPtr<ID3D11PixelShader>> pixelShader =
                this->shaderManager->getLoadedPixelShader(shader);
            if (!pixelShader)
            {
                spdlog::error("Pixel shader not loaded.");
                return false;
            }
            this->graphicsManager->deviceContext->PSSetShader(
                pixelShader.value().Get(), nullptr, 0
            );
            break;
        }
        default:
        {
            spdlog::error("Shader type not implemented.");
            return false;
        }
        }
    }

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

    std::vector<ShaderManager::Shader> shaderStack = {
        ShaderManager::Shader::STANDARD_VERTEX,
        ShaderManager::Shader::LIGHTING_PIXEL
    };

    this->drawMesh(
        stride, offset, this->triangleMesh, this->triangleMeshWorldMatrix,
        shaderStack
    );
    this->drawMesh(
        stride, offset, this->sphereMesh, this->sphereMeshWorldMatrix,
        shaderStack
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
    style.ScaleAllSizes(this->graphicsManager->config->window->mainScale);
    style.FontScaleDpi = this->graphicsManager->config->window->mainScale;

    ImGui_ImplWin32_Init(this->graphicsManager->config->window->hwnd);
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
