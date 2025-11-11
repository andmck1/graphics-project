#pragma once

#include "core_graphics/camera/camera.h"
#include "core_graphics/d3d/d3d.h"
#include "core_graphics/d3d/renderTexture.h"
#include "core_graphics/mesh/ortho.h"

#include "../shadermanager/shadermanager.h"
#include "core_graphics/sampler/base.h"
#include "core_graphics/shader/shader.h"

#include <array>

class RenderStage
{
    public:
    RenderTexture *inputRenderTexture;
    std::optional<RenderTexture *> outputRenderTexture = std::nullopt;
    BaseSampler * textureSampler;
    std::optional<OrthoMesh> mesh;
    std::array<float, 4> clearColour;
    Camera *camera;

    ShaderManagerState *shaderManagerState;
    PixelShader *pixelShader;

    bool render(D3D *graphicsManager);
};
