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

bool ClampSampler::setVertexShader(
    D3D *graphicsManager, BufferSlots *slotCounters
)
{
    int startSlot = slotCounters->vertexShaderCounters.samplerSlot;
    graphicsManager->deviceContext->VSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounters->vertexShaderCounters.samplerSlot++;
    return true;
}

bool ClampSampler::setPixelShader(
    D3D *graphicsManager, BufferSlots *slotCounters
)
{
    int startSlot = slotCounters->pixelShaderCounters.samplerSlot;
    graphicsManager->deviceContext->PSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounters->pixelShaderCounters.samplerSlot++;
    return true;
}
