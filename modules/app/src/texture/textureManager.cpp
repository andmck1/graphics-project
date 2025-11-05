#include "textureManager.h"

#include <spdlog/spdlog.h>

TextureManager::TextureManager(TextureManagerState *state)
{
    this->state = state;
}

bool TextureManager::loadAll()
{
    spdlog::info("Loading all textures");
    if (!this->state->defaultTexture)
        return false;

    return true;
}
