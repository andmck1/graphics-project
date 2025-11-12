#include "spotLights.h"
#include <core_utils/graphics.h>

bool SpotLightsBuffer::init(D3D *graphicsManager)
{
    spdlog::debug("Initialising spot lights buffer");
    HRESULT createBufferHR;
    HRESULT createShaderResourceViewHR;

    this->graphicsManager = graphicsManager;

    // Copilot help
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(SpotLightsBuffer::ShaderBuffer) *
                                 static_cast<UINT>(this->spotLights->size());
    this->bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    this->bufferDesc.StructureByteStride =
        sizeof(SpotLightsBuffer::ShaderBuffer);

    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create spot lights buffer."))
        return false;

    this->srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    this->srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    this->srvDesc.Buffer.FirstElement = 0;
    this->srvDesc.Buffer.NumElements = static_cast<UINT>(spotLights->size());

    createShaderResourceViewHR =
        this->graphicsManager->device->CreateShaderResourceView(
            this->buffer.Get(), &srvDesc,
            this->shaderResourceView.GetAddressOf()
        );
    if (!TryDXCall(
            createShaderResourceViewHR,
            "Failed to create SRV for spot lights buffer."
        ))
        return false;

    return true;
}

void SpotLightsBuffer::setData(std::vector<SpotLight *> *spotLights)
{
    this->spotLights = spotLights;
}

// Copilot help here
bool SpotLightsBuffer::setShader(BufferSlots *slotCounters, int numberOfBuffers)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    SpotLightsBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->pixelShaderCounters.textureSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Spot light buffers pixel shader map failed.\n"))
        return false;

    bufferData = reinterpret_cast<SpotLightsBuffer::ShaderBuffer *>(
        mappedResource.pData
    );

    for (int i = 0; i < this->spotLights->size(); i++)
    {
        bufferData[i].diffuseColour = XMFLOAT4(
            (*this->spotLights)[i]->diffuseColour[0],
            (*this->spotLights)[i]->diffuseColour[1],
            (*this->spotLights)[i]->diffuseColour[2], 0.f
        );
        bufferData[i].specularColour = XMFLOAT4(
            (*this->spotLights)[i]->specularColour[0],
            (*this->spotLights)[i]->specularColour[1],
            (*this->spotLights)[i]->specularColour[2], 0.f
        );
        bufferData[i].specularPower =
            XMFLOAT4((*this->spotLights)[i]->specularPower, 0.f, 0.f, 0.f);
        bufferData[i].worldPosition = XMFLOAT4(
            (*this->spotLights)[i]->worldPosition[0],
            (*this->spotLights)[i]->worldPosition[1],
            (*this->spotLights)[i]->worldPosition[2], 0.f
        );
        bufferData[i].directionVector = XMFLOAT4(
            (*this->spotLights)[i]->directionVector[0],
            (*this->spotLights)[i]->directionVector[1],
            (*this->spotLights)[i]->directionVector[2], 0.f
        );
        bufferData[i].coneAngle =
            XMFLOAT4((*this->spotLights)[i]->coneAngle, 0.f, 0.f, 0.f);
        bufferData[i].attenuationCoefficients = XMFLOAT4(
            (*this->spotLights)[i]->attenuationCoefficients[0],
            (*this->spotLights)[i]->attenuationCoefficients[1],
            (*this->spotLights)[i]->attenuationCoefficients[2], 0.f
        );
        bufferData[i].isDiffuseEnabled =
            XMINT4((int) (*this->spotLights)[i]->isDiffuseEnabled, 0, 0, 0);
        bufferData[i].isSpecularEnabled =
            XMINT4((int) (*this->spotLights)[i]->isSpecularEnabled, 0, 0, 0);
        bufferData[i].isAttenuationEnabled =
            XMINT4((int) (*this->spotLights)[i]->isAttenuationEnabled, 0, 0, 0);
        bufferData[i].nLights = XMINT4{(int) this->spotLights->size(), 0, 0, 0};
    }

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetShaderResources(
        bufferSlot, numberOfBuffers, this->shaderResourceView.GetAddressOf()
    );

    slotCounters->pixelShaderCounters.textureSlot++;

    return true;
}
