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
    int nDirectionalLights = 0;

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
        std::string iString = std::to_string(i);
        std::string pointLightHeading = "Point Light " + iString + " Settings";
        if (ImGui::CollapsingHeader(pointLightHeading.c_str()))
        {
            PointLight *pointLight = this->state->pointLights->at(i).get();
            ImGui::ColorPicker3(
                ("Diffuse#Point#" + iString).c_str(),
                pointLight->diffuseColour.get(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::ColorPicker3(
                ("Specular#Point#" + iString).c_str(),
                pointLight->specularColour.get(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::SliderFloat(
                ("Specular Power#Point#" + iString).c_str(),
                pointLight->specularPower.get(), 0.01f, 50.f, "%.3f", false
            );
            ImGui::SliderFloat3(
                ("World Position#Point#" + iString).c_str(),
                pointLight->worldPosition.get(), -10.f, 10.f
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C0#Point#" + iString).c_str(),
                &pointLight->attenuationCoefficients[0], 0.1f, 1.f, "%.3f",
                false
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C1#Point#" + iString).c_str(),
                &pointLight->attenuationCoefficients[1], 0.01f, .3f, "%.5f",
                false
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C2#Point#" + iString).c_str(),
                &pointLight->attenuationCoefficients[2], .01f, .1f, "%.6f",
                false
            );
            ImGui::Checkbox(
                ("Diffuse Enabled?#Point#" + iString).c_str(),
                pointLight->isDiffuseEnabled.get()
            );
            ImGui::Checkbox(
                ("Specular Enabled?#Point#" + iString).c_str(),
                pointLight->isSpecularEnabled.get()
            );
            ImGui::Checkbox(
                ("Attenuation Enabled?#Point#" + iString).c_str(),
                pointLight->isAttenuationEnabled.get()
            );
        }
    }

    nDirectionalLights = this->state->directionalLights->size();
    for (int i = 0; i < nDirectionalLights; i++)
    {
        std::string iString = std::to_string(i);
        std::string directionalLightHeading =
            "Directional Light " + iString + " Settings";
        if (ImGui::CollapsingHeader(directionalLightHeading.c_str()))
        {
            DirectionalLight *directionalLight =
                this->state->directionalLights->at(i).get();
            ImGui::ColorPicker3(
                ("Diffuse#Directional#" + iString).c_str(),
                directionalLight->diffuseColour.get(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::ColorPicker3(
                ("Specular#Directional#" + iString).c_str(),
                directionalLight->specularColour.get(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::SliderFloat(
                ("Specular Power#Directional#" + iString).c_str(),
                directionalLight->specularPower.get(), 0.01f, 50.f, "%.3f",
                false
            );
            ImGui::SliderFloat3(
                ("Direction Vector#Directional#" + iString).c_str(),
                directionalLight->directionVector.get(), -1.f, 1.f
            );
            ImGui::Checkbox(
                ("Diffuse Enabled?#Directional#" + iString).c_str(),
                directionalLight->isDiffuseEnabled.get()
            );
            ImGui::Checkbox(
                ("Specular Enabled?#Directional#" + iString).c_str(),
                directionalLight->isSpecularEnabled.get()
            );
        }
    }

    ImGui::SliderFloat(
        "Water Wave Magnitude", this->state->water->magnitude.get(), 0.1f, 10.f
    );
    ImGui::SliderFloat(
        "Water Wave Number", this->state->water->waveNumber.get(), 0.1f, 1.f
    );
    ImGui::SliderFloat(
        "Water Wave Angular Velocity",
        this->state->water->angularVelocity.get(), 0.1f, 10.f
    );

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return true;
}
