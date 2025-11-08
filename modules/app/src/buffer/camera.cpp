#include "cameraBuffer.h"
#include <core_utils/graphics.h>

bool CameraBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(CameraBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create ambient light buffer."))
        return false;
    return true;
}

bool CameraBuffer::setVertexShader(int bufferSlot, int numberOfBuffers)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CameraBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Camera buffer vertex shader map failed.\n"))
        return false;
    bufferData = (CameraBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->worldPosition = XMFLOAT4(
        this->camera->worldPosition[0], this->camera->worldPosition[1],
        this->camera->worldPosition[2], 0.f
    );

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->VSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );
    return true;
}
