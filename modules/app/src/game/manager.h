#include <d3d11.h>
#include <wrl/client.h>

#include "graphics/system/camera.h"
#include "scene.h"

struct GameManager
{
    GameManager(
        ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
    );

    Camera camera;
    Scene scene;
};
