#include "lightSettings.h"
#include <core_utils/graphics.h>

bool LightSettingsPixelBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising light settings data buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(LightSettingsPixelBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create light settings buffer."))
        return false;
    return true;
}

void LightSettingsPixelBuffer::setData(
    bool *isNormalsAsColourEnabled, float *shadowBias
)
{
    this->isNormalsAsColourEnabled = isNormalsAsColourEnabled;
    this->shadowBias = shadowBias;
}

bool LightSettingsPixelBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    LightSettingsPixelBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->pixelShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Light settings pixel shader map failed.\n"))
        return false;
    bufferData =
        (LightSettingsPixelBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->isNormalsAsColourEnabled =
        XMINT4((int) *(this->isNormalsAsColourEnabled), 0, 0, 0);
    bufferData->shadowBias = XMFLOAT4((float) *(this->shadowBias), 0, 0, 0);

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->pixelShaderCounters.constantSlot++;
    return true;
}
