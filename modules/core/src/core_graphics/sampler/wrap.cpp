#include "wrap.h"
#include "core_utils/graphics.h"

bool WrapSampler::init(D3D *graphicsManager)
{
    HRESULT createSamplerStateHR;
    D3D11_SAMPLER_DESC samplerDescription;

    samplerDescription.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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

bool WrapSampler::setVertexShader(
    D3D *graphicsManager, BufferSlots *slotCounter
)
{
    int startSlot = slotCounter->vertexShaderCounters.samplerSlot;
    graphicsManager->deviceContext->VSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounter->vertexShaderCounters.samplerSlot++;
    return true;
}

bool WrapSampler::setDomainShader(
    D3D *graphicsManager, BufferSlots *slotCounters
)
{
    int startSlot = slotCounters->domainShaderCounters.samplerSlot;
    graphicsManager->deviceContext->DSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounters->domainShaderCounters.samplerSlot++;
    return true;
}

bool WrapSampler::setPixelShader(D3D *graphicsManager, BufferSlots *slotCounter)
{
    int startSlot = slotCounter->pixelShaderCounters.samplerSlot;
    graphicsManager->deviceContext->PSSetSamplers(
        startSlot, 1, this->samplerState.GetAddressOf()
    );
    slotCounter->pixelShaderCounters.samplerSlot++;
    return true;
}
