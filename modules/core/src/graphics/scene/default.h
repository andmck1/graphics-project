#pragma once

#include "base.h"

class DefaultScene : public BaseScene
{
    public:
    DefaultScene(
        ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext
    );
};
