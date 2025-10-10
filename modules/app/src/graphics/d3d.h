#pragma once

#include <wrl.h>
#include <d3d11.h>

using namespace Microsoft::WRL;

class D3D
{
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
    ComPtr<IDXGISwapChain> swapChain;

    bool CreateDeviceAndSwapChain(int width, int height);
};
