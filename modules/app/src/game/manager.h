#include <d3d11.h>
#include <wrl/client.h>

#include "graphics/system/camera.h"
#include "graphics/scene/default.h"

struct GameManager
{
    template <typename TScene>
        requires(std::is_base_of_v<BaseScene, TScene>)
    GameManager(
        ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
        TScene scene
    );

    Camera camera;
    DefaultScene scene;
};
