#include "tessellationhulldata.h"
#include <core_utils/graphics.h>

bool TessellationHullDataBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising tessellationhull data buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth =
        sizeof(TessellationHullDataBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create tessellationhull buffer."))
        return false;
    return true;
}

void TessellationHullDataBuffer::setData(
    SharedBufferTypes::TessellationHullDataBufferType *settings
)
{
    this->settings = settings;
}

bool TessellationHullDataBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    TessellationHullDataBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->hullShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "TessellationHull hull shader map failed.\n"))
        return false;
    bufferData =
        (TessellationHullDataBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->minTessellationDistanceThreshold =
        this->settings->minTessellationDistanceThreshold;
    bufferData->maxTessellationDistanceThreshold =
        this->settings->maxTessellationDistanceThreshold;
    bufferData->minTessellationFactor = this->settings->minTessellationFactor;
    bufferData->maxTessellationFactor = this->settings->maxTessellationFactor;

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->HSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->hullShaderCounters.constantSlot++;
    return true;
}
