#pragma once

#include "core_graphics/texture/texture.h"
#include <memory>

struct TextureManagerState
{
    std::unique_ptr<DefaultTexture> defaultTexture;
    std::unique_ptr<FileTexture> moonTexture;
    std::unique_ptr<FileTexture> botwTexture;
};

class TextureManager
{
    public:
    TextureManager(TextureManagerState *state);

    TextureManagerState *state;
    bool loadAll();

};
