#pragma once

#include "core_graphics/texture/default.h"
#include "core_graphics/texture/file.h"

struct TextureManagerState
{
    DefaultTexture defaultTexture;
    FileTexture mountainsHeightMap;
    FileTexture mountainsTexture;
    FileTexture lightDebugTexture;
    FileTexture waterTexture;
};

class TextureManager
{
    public:
    TextureManager(TextureManagerState *state);

    TextureManagerState *state;
    bool loadAll();
};
