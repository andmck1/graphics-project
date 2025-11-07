#pragma once

#include "core_graphics/texture/default.h"
#include "core_graphics/texture/file.h"

struct TextureManagerState
{
    std::unique_ptr<DefaultTexture> defaultTexture;
    std::unique_ptr<FileTexture> moonTexture;
    std::unique_ptr<FileTexture> ukHeightMap;
    std::unique_ptr<FileTexture> mountainsHeightMap;
    std::unique_ptr<FileTexture> mountainsTexture;
    std::unique_ptr<FileTexture> lightDebugTexture;
};

class TextureManager
{
    public:
    TextureManager(TextureManagerState *state);

    TextureManagerState *state;
    bool loadAll();
};
