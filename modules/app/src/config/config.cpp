#include "config.h"

#include <filesystem>

bool Config::initWindowConfig()
{
    this->windowConfig.height = 720;
    this->windowConfig.width = 1280;
    this->windowConfig.windowName = "Graphics Project";

    return true;
}

bool Config::initD3DAppConfig(Window *window)
{
    this->d3dAppConfig.window = window;
    return true;
}

bool Config::initShaderManagerConfig(D3D *graphicsManager)
{
    this->shaderManagerConfig.graphicsManager = graphicsManager;
    this->shaderManagerConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    shaderManagerConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/hlsl/pixel").string();

    VertexShader standardVertexShader = VertexShader();
    standardVertexShader.filepath = "./build/hlsl/vertex/vertex.cso";
    standardVertexShader.type = ShaderType::VERTEX;
    standardVertexShader.inputElementDescription = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    this->shaderManagerConfig.shaders.push_back(
        std::make_unique<VertexShader>(standardVertexShader)
    );

    PixelShader lightingPixelShader = PixelShader();
    lightingPixelShader.filepath = "./build/hlsl/pixel/pixel.cso";
    lightingPixelShader.type = ShaderType::PIXEL;
    this->shaderManagerConfig.shaders.push_back(
        std::make_unique<PixelShader>(lightingPixelShader)
    );

    return true;
}

bool Config::initSceneConfig(
    D3D *graphicsManager, ShaderManager *shaderManager
)
{
    this->sceneConfig.graphicsManager = graphicsManager;
    this->sceneConfig.shaderManager = shaderManager;
    this->sceneConfig.vertexShaderDirectory =
        std::filesystem::absolute("./build/hlsl/vertex").string();
    this->sceneConfig.pixelShaderDirectory =
        std::filesystem::absolute("./build/hlsl/pixel").string();

    return true;
}
