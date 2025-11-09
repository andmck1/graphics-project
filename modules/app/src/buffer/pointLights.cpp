#include "pointLights.h"
#include <core_utils/graphics.h>

bool PointLightsBuffer::init(D3D *graphicsManager)
{
    spdlog::debug("Initialising point lights buffer");
    HRESULT createBufferHR;
    HRESULT createShaderResourceViewHR;

    this->graphicsManager = graphicsManager;

    // Copilot help
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(PointLightsBuffer::ShaderBuffer) *
                                 static_cast<UINT>(this->pointLights->size());
    this->bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    this->bufferDesc.StructureByteStride =
        sizeof(PointLightsBuffer::ShaderBuffer);

    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create point lights buffer."))
        return false;

    this->srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    this->srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    this->srvDesc.Buffer.FirstElement = 0;
    this->srvDesc.Buffer.NumElements = static_cast<UINT>(pointLights->size());

    createShaderResourceViewHR =
        this->graphicsManager->device->CreateShaderResourceView(
            this->buffer.Get(), &srvDesc,
            this->shaderResourceView.GetAddressOf()
        );
    if (!TryDXCall(
            createShaderResourceViewHR,
            "Failed to create SRV for point lights buffer."
        ))
        return false;

    return true;
}

void PointLightsBuffer::setData(std::vector<PointLight *> *pointLights)
{
    this->pointLights = pointLights;
}

// Copilot help here
bool PointLightsBuffer::setShader(BufferSlots *slotCounters, int numberOfBuffers)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    PointLightsBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->pixelShaderCounters.textureSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Point light buffers pixel shader map failed.\n"))
        return false;

    bufferData = reinterpret_cast<PointLightsBuffer::ShaderBuffer *>(
        mappedResource.pData
    );

    for (int i = 0; i < this->pointLights->size(); i++)
    {
        bufferData[i].diffuseColour = XMFLOAT4(
            (*this->pointLights)[i]->diffuseColour[0],
            (*this->pointLights)[i]->diffuseColour[1],
            (*this->pointLights)[i]->diffuseColour[2], 0.f
        );
        bufferData[i].specularColour = XMFLOAT4(
            (*this->pointLights)[i]->specularColour[0],
            (*this->pointLights)[i]->specularColour[1],
            (*this->pointLights)[i]->specularColour[2], 0.f
        );
        bufferData[i].specularPower =
            XMFLOAT4((*this->pointLights)[i]->specularPower, 0.f, 0.f, 0.f);
        bufferData[i].worldPosition = XMFLOAT4(
            (*this->pointLights)[i]->worldPosition[0],
            (*this->pointLights)[i]->worldPosition[1],
            (*this->pointLights)[i]->worldPosition[2], 0.f
        );
        bufferData[i].attenuationCoefficients = XMFLOAT4(
            (*this->pointLights)[i]->attenuationCoefficients[0],
            (*this->pointLights)[i]->attenuationCoefficients[1],
            (*this->pointLights)[i]->attenuationCoefficients[2], 0.f
        );
        bufferData[i].isDiffuseEnabled =
            XMINT4((int) (*this->pointLights)[i]->isDiffuseEnabled, 0, 0, 0);
        bufferData[i].isSpecularEnabled =
            XMINT4((int) (*this->pointLights)[i]->isSpecularEnabled, 0, 0, 0);
        bufferData[i].isAttenuationEnabled = XMINT4(
            (int) (*this->pointLights)[i]->isAttenuationEnabled, 0, 0, 0
        );
        bufferData[i].nLights =
            XMINT4{(int) this->pointLights->size(), 0, 0, 0};
    }

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetShaderResources(
        bufferSlot, numberOfBuffers, this->shaderResourceView.GetAddressOf()
    );
    
    slotCounters->pixelShaderCounters.textureSlot++;

    return true;
}
