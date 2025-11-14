#include "texturemanager.h"

#include <spdlog/spdlog.h>

TextureManager::TextureManager(TextureManagerState *state)
{
    this->state = state;
}

bool TextureManager::loadAll()
{
    // TODO - make this loop
    spdlog::info("Loading all textures");
    if (!this->state->defaultTexture.load())
        return false;
    if (!this->state->moonTexture.load())
        return false;
    if (!this->state->mountainsHeightMap.load())
        return false;
    if (!this->state->mountainsTexture.load())
        return false;
    if (!this->state->lightDebugTexture.load())
        return false;
    if (!this->state->waterTexture.load())
        return false;

    return true;
}
