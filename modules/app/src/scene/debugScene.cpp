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
    int nSpotLights = 0;
    int nDirectionalLights = 0;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Draggable Window");
    ImGui::Text("FPS: %.2f", *(this->state->fps));
    ImGui::Checkbox("Wireframe Enabled?", this->state->isWireframeEnabled);
    ImGui::Checkbox(
        "Normals As Colour Enabled?", this->state->isNormalsAsColourEnabled
    );
    if (ImGui::CollapsingHeader("Ambient Light Settings"))
    {
        ImGui::ColorPicker3(
            "Colour", this->state->ambientLight->colour.data(),
            ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

        );
        ImGui::Checkbox(
            "Ambient Enabled?", &this->state->ambientLight->isEnabled
        );
    }

    nPointLights = this->state->pointLights->size();
    for (int i = 0; i < nPointLights; i++)
    {
        std::string iString = std::to_string(i);
        std::string pointLightHeading = "Point Light " + iString + " Settings";
        if (ImGui::CollapsingHeader(pointLightHeading.c_str()))
        {
            PointLight *pointLight = this->state->pointLights->at(i);
            ImGui::ColorPicker3(
                ("Diffuse#Point#" + iString).c_str(),
                pointLight->diffuseColour.data(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::ColorPicker3(
                ("Specular#Point#" + iString).c_str(),
                pointLight->specularColour.data(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::SliderFloat(
                ("Specular Power#Point#" + iString).c_str(),
                &pointLight->specularPower, 0.01f, 50.f, "%.3f", false
            );
            ImGui::SliderFloat3(
                ("World Position#Point#" + iString).c_str(),
                pointLight->worldPosition.data(), -2000.f, 2000.f
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C0#Point#" + iString).c_str(),
                &pointLight->attenuationCoefficients[0], 0.00001f, .001f,
                "%.5f", false
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C1#Point#" + iString).c_str(),
                &pointLight->attenuationCoefficients[1], 0.00001f, .001f,
                "%.8f", false
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C2#Point#" + iString).c_str(),
                &pointLight->attenuationCoefficients[2], 1e-7, 1e-5f,
                "%.8f", false
            );
            ImGui::Checkbox(
                ("Diffuse Enabled?#Point#" + iString).c_str(),
                &pointLight->isDiffuseEnabled
            );
            ImGui::Checkbox(
                ("Specular Enabled?#Point#" + iString).c_str(),
                &pointLight->isSpecularEnabled
            );
            ImGui::Checkbox(
                ("Attenuation Enabled?#Point#" + iString).c_str(),
                &pointLight->isAttenuationEnabled
            );
        }
    }

    nSpotLights = this->state->spotLights->size();
    for (int i = 0; i < nSpotLights; i++)
    {
        std::string iString = std::to_string(i);
        std::string spotLightHeading = "Spot Light " + iString + " Settings";
        if (ImGui::CollapsingHeader(spotLightHeading.c_str()))
        {
            SpotLight *spotLight = this->state->spotLights->at(i);
            ImGui::ColorPicker3(
                ("Diffuse#Spot#" + iString).c_str(),
                spotLight->diffuseColour.data(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::ColorPicker3(
                ("Specular#Spot#" + iString).c_str(),
                spotLight->specularColour.data(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::SliderFloat(
                ("Specular Power#Spot#" + iString).c_str(),
                &spotLight->specularPower, 0.01f, 50.f, "%.3f", false
            );
            ImGui::SliderFloat3(
                ("World Position#Spot#" + iString).c_str(),
                spotLight->worldPosition.data(), -2000.f, 2000.f
            );
            ImGui::SliderFloat(
                ("Cone Angle#Spot#" + iString).c_str(), &spotLight->coneAngle,
                0.1f, 180.f
            );
            ImGui::SliderFloat3(
                ("Direction Vector#Directional#" + iString).c_str(),
                spotLight->directionVector.data(), -1.f, 1.f
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C0#Point#" + iString).c_str(),
                &spotLight->attenuationCoefficients[0], 0.00001f, .001f,
                "%.8f", false
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C1#Point#" + iString).c_str(),
                &spotLight->attenuationCoefficients[1], 0.00001f, .001f,
                "%.8f", false
            );
            ImGui::SliderFloat(
                ("Attenuation Coefficient C2#Point#" + iString).c_str(),
                &spotLight->attenuationCoefficients[2], 1e-7f, 1e-5f,
                "%.8f", false
            );

            ImGui::Checkbox(
                ("Diffuse Enabled?#Spot#" + iString).c_str(),
                &spotLight->isDiffuseEnabled
            );
            ImGui::Checkbox(
                ("Specular Enabled?#Spot#" + iString).c_str(),
                &spotLight->isSpecularEnabled
            );
            ImGui::Checkbox(
                ("Attenuation Enabled?#Spot#" + iString).c_str(),
                &spotLight->isAttenuationEnabled
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
                this->state->directionalLights->at(i);
            ImGui::ColorPicker3(
                ("Diffuse#Directional#" + iString).c_str(),
                directionalLight->diffuseColour.data(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::ColorPicker3(
                ("Specular#Directional#" + iString).c_str(),
                directionalLight->specularColour.data(),
                ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB

            );
            ImGui::SliderFloat(
                ("Specular Power#Directional#" + iString).c_str(),
                &directionalLight->specularPower, 0.01f, 50.f, "%.3f", false
            );
            ImGui::SliderFloat3(
                ("Direction Vector#Directional#" + iString).c_str(),
                directionalLight->directionVector.data(), -1.f, 1.f
            );
            ImGui::Checkbox(
                ("Diffuse Enabled?#Directional#" + iString).c_str(),
                &directionalLight->isDiffuseEnabled
            );
            ImGui::Checkbox(
                ("Specular Enabled?#Directional#" + iString).c_str(),
                &directionalLight->isSpecularEnabled
            );
        }
    }

    if (ImGui::CollapsingHeader("Water settings"))
    {
        ImGui::SliderFloat(
            "Water Wave Magnitude", &this->state->water->magnitude, 0.1f, 2.f
        );
        ImGui::SliderFloat(
            "Water Wave Number", &this->state->water->waveNumber, 0.1f, 10.f
        );
        ImGui::SliderFloat(
            "Water Wave Angular Velocity", &this->state->water->angularVelocity,
            0.1f, 10.f
        );
        ImGui::SliderFloat(
            "Water Steepness", &this->state->water->steepness,
            0.1f, 10.f
        );
    }

    if (ImGui::CollapsingHeader("Postprocessor settings"))
    {

        ImGui::Checkbox("Is CRT Enabled?", this->state->isCRTEnabled);

        ImGui::Checkbox(
            "Enable VHS Wiggle",
            (bool *) &this->state->crtSettings->isVHSWiggleEnabled.x
        );
        ImGui::Checkbox(
            "Enable Curving",
            (bool *) &this->state->crtSettings->isCurvingEnabled.x
        );
        ImGui::Checkbox(
            "Enable Chromatic Aberration",
            (bool *) &this->state->crtSettings->isChromaticAberrationEnabled.x
        );
        ImGui::Checkbox(
            "Enable Grain", (bool *) &this->state->crtSettings->isGrainEnabled.x
        );
        ImGui::Checkbox(
            "Enable RGB Grid",
            (bool *) &this->state->crtSettings->isRGBGridEnabled.x
        );
        ImGui::Checkbox(
            "Enable Scanlines",
            (bool *) &this->state->crtSettings->isScanlinesEnabled.x
        );
        ImGui::Checkbox(
            "Enable Vingette",
            (bool *) &this->state->crtSettings->isVingetteEnabled.x
        );

        ImGui::Separator();

        ImGui::SliderFloat(
            "Brightness Multiplier",
            &this->state->crtSettings->brightnessMultiplier.x, 0.1f, 3.0f
        );
        ImGui::SliderFloat(
            "Grain Strength", &this->state->crtSettings->grainStrength.x, 0.1f,
            1.0f
        );
        ImGui::SliderFloat(
            "Curve Power", &this->state->crtSettings->curvePower.x, 0.1f, 2.0f
        );
        ImGui::SliderFloat(
            "Vingette Strength", &this->state->crtSettings->vingetteStrength.x,
            0.1f, 2.0f
        );
        ImGui::SliderInt(
            "Scanlines Interval",
            &this->state->crtSettings->scanlinesInterval.x, 2, 10
        );
        ImGui::SliderFloat(
            "Scanlines Opacity", &this->state->crtSettings->scanlinesOpacity.x,
            0.1f, 1.0f
        );
        ImGui::SliderInt(
            "Scanlines Thickness",
            &this->state->crtSettings->scanlinesThickness.x, 1, 5
        );
        ImGui::SliderFloat(
            "Chromatic Aberration Strength",
            &this->state->crtSettings->chromaticAberrationStrength.x, 0.1f, 4.0f
        );
        ImGui::SliderFloat(
            "VHS Wiggle Amount", &this->state->crtSettings->vhsWiggle.x, 0.001f,
            0.005f
        );
    }

    if (ImGui::CollapsingHeader("Tessellation settings"))
    {
        ImGui::SliderFloat(
            "Min Distance for Maximum Tessellation Factor",
            &this->state->tessellationSettings->minTessellationDistanceThreshold
                 .x,
            10.f, 100.f
        );
        ImGui::SliderFloat(
            "Max Distance for Minimum Tessellation Factor",
            &this->state->tessellationSettings->maxTessellationDistanceThreshold
                 .x,
            80.f, 5000.f
        );
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return true;
}
