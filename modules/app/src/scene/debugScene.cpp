#include "debugScene.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

DebugScene::DebugScene(DebugSceneConfig *config)
{
    this->config = config;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(
        this->config->graphicsManager->config->window->mainScale
    );
    style.FontScaleDpi =
        this->config->graphicsManager->config->window->mainScale;

    ImGui_ImplWin32_Init(this->config->graphicsManager->config->window->hwnd);
    ImGui_ImplDX11_Init(
        this->config->graphicsManager->device.Get(),
        this->config->graphicsManager->deviceContext.Get()
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
    ImGui::Text("FPS: %.2f", *(this->config->fps));
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return true;
}
