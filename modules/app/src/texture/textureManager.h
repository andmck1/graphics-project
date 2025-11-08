#pragma once

#include "core_graphics/texture/default.h"
#include "core_graphics/texture/file.h"

struct TextureManagerState
{
    TextureManagerState();
    DefaultTexture defaultTexture;
    FileTexture moonTexture;
    FileTexture mountainsHeightMap;
    FileTexture mountainsTexture;
    FileTexture lightDebugTexture;
};

class TextureManager
{
    public:
    TextureManager(TextureManagerState *state);

    TextureManagerState *state;
    bool loadAll();
};
