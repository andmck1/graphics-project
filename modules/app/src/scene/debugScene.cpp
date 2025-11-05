#include "debugScene.h"

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

DebugScene::DebugScene(DebugSceneState *state)
{
    this->state = state;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(this->state->graphicsManager->state->window->mainScale);
    style.FontScaleDpi = this->state->graphicsManager->state->window->mainScale;

    ImGui_ImplWin32_Init(this->state->graphicsManager->state->window->hwnd);
    ImGui_ImplDX11_Init(
        this->state->graphicsManager->device.Get(),
        this->state->graphicsManager->deviceContext.Get()
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
    ImGui::Text("FPS: %.2f", *(this->state->fps));
    ImGui::Checkbox("Show Light Meshes?", this->state->isLightMeshesEnabled);
    if (ImGui::CollapsingHeader("Ambient Light Settings"))
    {
        ImGui::ColorPicker3(
            "Colour", this->state->ambientLightColour,
            ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

        );
        ImGui::Checkbox("Ambient Enabled?", this->state->isAmbientLightEnabled);
    }
    if (ImGui::CollapsingHeader("Point Light Settings"))
    {
        ImGui::ColorPicker3(
            "Diffuse", this->state->pointLightDiffuseColour,
            ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

        );
        ImGui::ColorPicker3(
            "Specular", this->state->pointLightSpecularColour,
            ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

        );
        ImGui::SliderFloat(
            "Specular Power", this->state->pointLightSpecularPower, 0.01f, 50.f,
            "%.3f", false
        );
        ImGui::SliderFloat3(
            "World Position", this->state->pointLightWorldPosition, -1000.f,
            1000.f
        );
        ImGui::SliderFloat(
            "Attenuation Coefficient C0",
            &this->state->pointLightAttenuationCoefficients[0], 0.1f, 1.f,
            "%.3f", false
        );
        ImGui::SliderFloat(
            "Attenuation Coefficient C1",
            &this->state->pointLightAttenuationCoefficients[1], 0.0001f, 0.001f,
            "%.5f", true
        );
        ImGui::SliderFloat(
            "Attenuation Coefficient C2",
            &this->state->pointLightAttenuationCoefficients[2], 0.00001f,
            0.0001f, "%.6f", true
        );
        ImGui::Checkbox(
            "Diffuse Enabled?", this->state->isPointLightDiffuseEnabled
        );
        ImGui::Checkbox(
            "Specular Enabled?", this->state->isPointLightSpecularEnabled
        );
        ImGui::Checkbox(
            "Attenuation Enabled?", this->state->isPointLightAttenuationEnabled
        );
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return true;
}
