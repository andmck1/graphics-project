#include "debugScene.h"
#include "core_graphics/light/point.h"

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
    int nPointLights = 0;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Draggable Window");
    ImGui::Text("FPS: %.2f", *(this->state->fps));
    if (ImGui::CollapsingHeader("Ambient Light Settings"))
    {
        ImGui::ColorPicker3(
            "Colour", this->state->ambientLight->colour.get(),
            ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

        );
        ImGui::Checkbox(
            "Ambient Enabled?", this->state->ambientLight->isEnabled.get()
        );
    }

    nPointLights = this->state->pointLights->size();
    for (int i = 0; i < nPointLights; i++)
    {
        std::string pointLightHeading =
            "Point Light " + std::to_string(i) + " Settings";
        if (ImGui::CollapsingHeader(pointLightHeading.c_str()))
        {
            PointLight *pointLight = this->state->pointLights->at(i).get();
            ImGui::ColorPicker3(
                "Diffuse", pointLight->diffuseColour.get(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::ColorPicker3(
                "Specular", pointLight->specularColour.get(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::SliderFloat(
                "Specular Power", pointLight->specularPower.get(), 0.01f, 50.f,
                "%.3f", false
            );
            ImGui::SliderFloat3(
                "World Position", pointLight->worldPosition.get(), -1000.f,
                1000.f
            );
            ImGui::SliderFloat(
                "Attenuation Coefficient C0",
                &pointLight->attenuationCoefficients[0], 0.1f, 1.f, "%.3f",
                false
            );
            ImGui::SliderFloat(
                "Attenuation Coefficient C1",
                &pointLight->attenuationCoefficients[1], 0.0001f, 0.001f,
                "%.5f", true
            );
            ImGui::SliderFloat(
                "Attenuation Coefficient C2",
                &pointLight->attenuationCoefficients[2], 0.00001f, 0.0001f,
                "%.6f", true
            );
            ImGui::Checkbox(
                "Diffuse Enabled?", pointLight->isDiffuseEnabled.get()
            );
            ImGui::Checkbox(
                "Specular Enabled?", pointLight->isSpecularEnabled.get()
            );
            ImGui::Checkbox(
                "Attenuation Enabled?", pointLight->isSpecularEnabled.get()
            );
        }
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return true;
}
