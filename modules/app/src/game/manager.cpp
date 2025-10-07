#include "manager.h"
#include "graphics/scene/base.h"
#include <type_traits>

template <typename TScene>
requires(std::is_base_of_v<BaseScene, TScene>)
GameManager::GameManager(
    ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext,
    TScene scene
)
{
    this->camera = Camera();
    this->scene = scene; 
}
