#include "pointLightBuffer.h"
#include <core_utils/graphics.h>

bool PointLightBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(PointLightBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create point light buffer."))
        return false;
    return true;
}

bool PointLightBuffer::setPixelShader()
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    PointLightBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Point light buffer pixel shader map failed.\n"))
        return false;
    bufferData = (PointLightBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->colour = pointLight->colour;
    bufferData->worldPosition = pointLight->worldPosition;
    bufferData->direction = pointLight->direction;

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetConstantBuffers(
        0, 1, this->buffer.GetAddressOf()
    );
    return true;
}
