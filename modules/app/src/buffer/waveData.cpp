#include "waveData.h"
#include <core_utils/graphics.h>

bool WaveDataVertexBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising wave data buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(WaveDataVertexBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create wave buffer."))
        return false;
    return true;
}

void WaveDataVertexBuffer::setData(
    float *totalTime, float *magnitude, float *waveNumber,
    float *angularVelocity, float *steepness
)
{
    this->totalTime = totalTime;
    this->magnitude = magnitude;
    this->waveNumber = waveNumber;
    this->angularVelocity = angularVelocity;
    this->steepness = steepness;
}

bool WaveDataVertexBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    WaveDataVertexBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->vertexShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Wave vertex shader map failed.\n"))
        return false;
    bufferData = (WaveDataVertexBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->totalTime = XMFLOAT4(*this->totalTime, 0.f, 0.f, 0.f);
    bufferData->magnitude = XMFLOAT4(*this->magnitude, 0.f, 0.f, 0.f);
    bufferData->waveNumber = XMFLOAT4(*this->waveNumber, 0.f, 0.f, 0.f);
    bufferData->angularVelocity =
        XMFLOAT4(*this->angularVelocity, 0.f, 0.f, 0.f);
    bufferData->steepness = XMFLOAT4(*this->steepness, 0.f, 0.f, 0.f);

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->VSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->vertexShaderCounters.constantSlot++;
    return true;
}

bool WaveDataDomainBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising wave data buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(WaveDataDomainBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create wave buffer."))
        return false;
    return true;
}

void WaveDataDomainBuffer::setData(
    float *totalTime, float *magnitude, float *waveNumber,
    float *angularVelocity, float *steepness
)
{
    this->totalTime = totalTime;
    this->magnitude = magnitude;
    this->waveNumber = waveNumber;
    this->angularVelocity = angularVelocity;
    this->steepness = steepness;
}

bool WaveDataDomainBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    WaveDataDomainBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->domainShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Wave domain shader map failed.\n"))
        return false;
    bufferData = (WaveDataDomainBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->totalTime = XMFLOAT4(*this->totalTime, 0.f, 0.f, 0.f);
    bufferData->magnitude = XMFLOAT4(*this->magnitude, 0.f, 0.f, 0.f);
    bufferData->waveNumber = XMFLOAT4(*this->waveNumber, 0.f, 0.f, 0.f);
    bufferData->angularVelocity =
        XMFLOAT4(*this->angularVelocity, 0.f, 0.f, 0.f);
    bufferData->steepness = XMFLOAT4(*this->steepness, 0.f, 0.f, 0.f);

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->DSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->domainShaderCounters.constantSlot++;
    return true;
}
