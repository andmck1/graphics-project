#include "upsampler.h"
#include "core_utils/graphics.h"

bool UpSampler::init(D3D *graphicsManager)
{
    HRESULT createSamplerStateHR;
    D3D11_SAMPLER_DESC samplerDescription;

    // (Microsoft Copilot, 2025)
    samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.MipLODBias = 0.f;
    samplerDescription.MaxAnisotropy = 16;

    createSamplerStateHR = graphicsManager->device->CreateSamplerState(
        &samplerDescription, this->samplerState.GetAddressOf()
    );
    return TryDXCall(createSamplerStateHR, "Failed to create up sampler state");
}

bool UpSampler::setVertexShader(D3D *graphicsManager, BufferSlots *slotCounters)
{
    int startSlot = slotCounters->vertexShaderCounters.samplerSlot;
    graphicsManager->deviceContext->VSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounters->vertexShaderCounters.samplerSlot++;
    return true;
}

bool UpSampler::setDomainShader(D3D *graphicsManager, BufferSlots *slotCounters)
{
    int startSlot = slotCounters->domainShaderCounters.samplerSlot;
    graphicsManager->deviceContext->DSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounters->domainShaderCounters.samplerSlot++;
    return true;
}

bool UpSampler::setPixelShader(D3D *graphicsManager, BufferSlots *slotCounters)
{
    int startSlot = slotCounters->pixelShaderCounters.samplerSlot;
    graphicsManager->deviceContext->PSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounters->pixelShaderCounters.samplerSlot++;
    return true;
}
