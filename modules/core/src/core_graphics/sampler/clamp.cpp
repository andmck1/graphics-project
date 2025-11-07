#include "clamp.h"
#include "core_utils/graphics.h"

bool ClampSampler::init(D3D *graphicsManager)
{
    HRESULT createSamplerStateHR;
    D3D11_SAMPLER_DESC samplerDescription;

    samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.MipLODBias = -0.5f;
    samplerDescription.MaxAnisotropy = 16;
    samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDescription.MinLOD = 0;
    samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

    createSamplerStateHR = graphicsManager->device->CreateSamplerState(
        &samplerDescription, this->samplerState.GetAddressOf()
    );
    return TryDXCall(createSamplerStateHR, "Failed to create sampler state");
}

bool ClampSampler::setVertexShader(D3D *graphicsManager)
{
    graphicsManager->deviceContext->VSGetSamplers(
        0, 1, this->samplerState.GetAddressOf()
    );
    return true;
}

bool ClampSampler::setPixelShader(D3D *graphicsManager)
{
    graphicsManager->deviceContext->PSSetSamplers(
        0, 1, this->samplerState.GetAddressOf()
    );
    return true;
}
