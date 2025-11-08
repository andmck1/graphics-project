#include "waveDataBuffer.h"
#include <core_utils/graphics.h>

bool WaveDataBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(WaveDataBuffer::ShaderBuffer);
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

bool WaveDataBuffer::setVertexShader(int bufferSlot, int numberOfBuffers)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    WaveDataBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Wave vertex shader map failed.\n"))
        return false;
    bufferData = (WaveDataBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->totalTime = XMFLOAT4(*this->totalTime, 0.f, 0.f, 0.f);
    bufferData->magnitude = XMFLOAT4(*this->magnitude, 0.f, 0.f, 0.f);
    bufferData->waveNumber = XMFLOAT4(*this->waveNumber, 0.f, 0.f, 0.f);
    bufferData->angularVelocity =
        XMFLOAT4(*this->angularVelocity, 0.f, 0.f, 0.f);

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->VSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );
    return true;
}
