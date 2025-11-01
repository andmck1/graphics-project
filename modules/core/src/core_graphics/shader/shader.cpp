#include "shader.h"

#include <filesystem>
#include <spdlog/spdlog.h>

#include <core_utils/data.h>

bool BaseShader::loadBlob(D3D *graphicsManager)
{
    const std::filesystem::path shaderFilepathAbsolute =
        std::filesystem::absolute(std::filesystem::path(this->filepath));
    const std::string shaderFilepathString =
        shaderFilepathAbsolute.generic_string();

    spdlog::info("Loading shader: " + shaderFilepathString);
    this->blob = graphicsManager->loadShader(shaderFilepathString);
    if (!(this->blob))
        return false;

    return true;
}
