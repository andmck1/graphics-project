#include "ambientLight.h"
#include <core_utils/graphics.h>

bool AmbientLightBuffer::init(D3D *graphicsManager)
{
    spdlog::debug("Initialising ambient light buffer");
    HRESULT createBufferHR;

    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(AmbientLightBuffer::ShaderBuffer);
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

void AmbientLightBuffer::setData(AmbientLight *ambientLight)
{
    this->ambientLight = ambientLight;
}

bool AmbientLightBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    AmbientLightBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;
    int bufferSlot = slotCounters->pixelShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Ambient light buffer pixel shader map failed.\n"))
        return false;
    bufferData = (AmbientLightBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->colour = XMFLOAT4(
        this->ambientLight->colour[0], ambientLight->colour[1],
        this->ambientLight->colour[2], 0.f
    );
    bufferData->isEnabled =
        XMINT4((int) this->ambientLight->isEnabled, 0, 0, 0);
    
    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->pixelShaderCounters.constantSlot++;
    
    return true;
}
