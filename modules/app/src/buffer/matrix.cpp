#include "matrix.h"
#include <core_utils/graphics.h>

bool MatrixBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(MatrixBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to matrix buffer."))
        return false;
    return true;
}

bool MatrixBuffer::setVertexShader(int bufferSlot, int numberOfBuffers)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Matrix buffer map failed.\n"))
        return false;
    bufferData = (MatrixBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->worldMatrix = *this->worldMatrix;
    bufferData->projectionMatrix = *this->projectionMatrix;
    bufferData->viewMatrix = *this->viewMatrix;
    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->VSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );
    return true;
}
