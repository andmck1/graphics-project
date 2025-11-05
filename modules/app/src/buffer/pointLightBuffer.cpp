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

bool PointLightBuffer::setPixelShader(int bufferSlot, int numberOfBuffers)
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
    bufferData->type = XMINT4{LightingShader::LIGHT_TYPE_POINT, 0, 0, 0};
    bufferData->diffuseColour = XMFLOAT4(
        this->pointLight->diffuseColour[0], pointLight->diffuseColour[1],
        this->pointLight->diffuseColour[2], 0.f
    );
    bufferData->specularColour = XMFLOAT4(
        this->pointLight->specularColour[0], pointLight->specularColour[1],
        this->pointLight->specularColour[2], 0.f
    );
    bufferData->specularPower =
        XMFLOAT4(*this->pointLight->specularPower, 0.f, 0.f, 0.f);
    bufferData->worldPosition = XMFLOAT4(
        this->pointLight->worldPosition[0], pointLight->worldPosition[1],
        this->pointLight->worldPosition[2], 0.f
    );
    bufferData->attenuationCoefficients = XMFLOAT4(
        this->pointLight->attenuationCoefficients[0],
        this->pointLight->attenuationCoefficients[1],
        this->pointLight->attenuationCoefficients[2], 0.f
    );
    bufferData->isDiffuseEnabled =
        XMINT4((int) *this->pointLight->isDiffuseEnabled, 0, 0, 0);
    bufferData->isSpecularEnabled =
        XMINT4((int) *this->pointLight->isSpecularEnabled, 0, 0, 0);
    bufferData->isAttenuationEnabled =
        XMINT4((int) *this->pointLight->isAttenuationEnabled, 0, 0, 0);

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );
    return true;
}
