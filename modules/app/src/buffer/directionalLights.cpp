#include "directionalLights.h"
#include <core_utils/graphics.h>

bool DirectionalLightsBuffer::init(D3D *graphicsManager)
{
    spdlog::debug("Initialising directional lights buffer");

    HRESULT createBufferHR;
    HRESULT createShaderResourceViewHR;

    this->graphicsManager = graphicsManager;

    // (Microsoft Copilot, 2025)
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth =
        sizeof(DirectionalLightsBuffer::ShaderBuffer) *
        static_cast<UINT>(this->directionalLights->size());
    this->bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    this->bufferDesc.StructureByteStride =
        sizeof(DirectionalLightsBuffer::ShaderBuffer);

    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(
            createBufferHR, "Failed to create directional lights buffer."
        ))
        return false;

    this->srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    this->srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    this->srvDesc.Buffer.FirstElement = 0;
    this->srvDesc.Buffer.NumElements =
        static_cast<UINT>(directionalLights->size());

    createShaderResourceViewHR =
        this->graphicsManager->device->CreateShaderResourceView(
            this->buffer.Get(), &srvDesc,
            this->shaderResourceView.GetAddressOf()
        );
    if (!TryDXCall(
            createShaderResourceViewHR,
            "Failed to create SRV for directional lights buffer."
        ))
        return false;

    return true;
}

void DirectionalLightsBuffer::setData(
    std::vector<DirectionalLight *> *directionalLights
)
{
    this->directionalLights = directionalLights;
}

// (Microsoft Copilot, 2025)
bool DirectionalLightsBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    DirectionalLightsBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->pixelShaderCounters.textureSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(
            mapHR, "Directional light buffers pixel shader map failed.\n"
        ))
        return false;

    bufferData = reinterpret_cast<DirectionalLightsBuffer::ShaderBuffer *>(
        mappedResource.pData
    );

    for (int i = 0; i < this->directionalLights->size(); i++)
    {
        bufferData[i].diffuseColour = XMFLOAT4(
            (*this->directionalLights)[i]->diffuseColour[0],
            (*this->directionalLights)[i]->diffuseColour[1],
            (*this->directionalLights)[i]->diffuseColour[2], 0.f
        );
        bufferData[i].specularColour = XMFLOAT4(
            (*this->directionalLights)[i]->specularColour[0],
            (*this->directionalLights)[i]->specularColour[1],
            (*this->directionalLights)[i]->specularColour[2], 0.f
        );
        bufferData[i].specularPower = XMFLOAT4(
            (*this->directionalLights)[i]->specularPower, 0.f, 0.f, 0.f
        );
        bufferData[i].directionVector = XMFLOAT4(
            (*this->directionalLights)[i]->directionVector[0],
            (*this->directionalLights)[i]->directionVector[1],
            (*this->directionalLights)[i]->directionVector[2], 0.f
        );
        bufferData[i].isDiffuseEnabled = XMINT4(
            (int) (*this->directionalLights)[i]->isDiffuseEnabled, 0, 0, 0
        );
        bufferData[i].isSpecularEnabled = XMINT4(
            (int) (*this->directionalLights)[i]->isSpecularEnabled, 0, 0, 0
        );
        bufferData[i].nLights =
            XMINT4{(int) this->directionalLights->size(), 0, 0, 0};
    }

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetShaderResources(
        bufferSlot, numberOfBuffers, this->shaderResourceView.GetAddressOf()
    );

    slotCounters->pixelShaderCounters.textureSlot++;
    return true;
}
