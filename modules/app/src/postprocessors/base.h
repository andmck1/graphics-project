#pragma once

#include "core_graphics/d3d/rendertexture.h"
#include "renderstage.h"

#include <vector>
#include <memory>

struct BasePostProcessor
{
    std::vector<std::unique_ptr<RenderTexture>> renderTextures;
    std::vector<std::unique_ptr<RenderStage>> renderStages;
    bool isEnabled = false;
};
