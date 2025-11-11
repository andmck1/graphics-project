#include "crt.h"
#include <core_utils/graphics.h>

bool CRTBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising crt buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(CRTBuffer::ShaderBuffer);
    this->bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    this->bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    this->bufferDesc.MiscFlags = 0;
    this->bufferDesc.StructureByteStride = 0;
    createBufferHR = this->graphicsManager->device->CreateBuffer(
        &(this->bufferDesc), NULL, this->buffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create crt buffer."))
        return false;
    return true;
}

void CRTBuffer::setData(
    SharedBufferTypes::CRTBufferType *inputBufferData, float *totalTime
)
{
    this->bufferDataRef = inputBufferData;
    this->totalTime = totalTime;
}

bool CRTBuffer::setShader(BufferSlots *slotCounters, int numberOfBuffers)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CRTBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->pixelShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "CRT buffer pixel shader map failed.\n"))
        return false;
    bufferData = (CRTBuffer::ShaderBuffer *) mappedResource.pData;

    bufferData->totalTime = XMFLOAT4(*this->totalTime, 0.f, 0.f, 0.f);
    
    bufferData->isVHSWiggleEnabled = this->bufferDataRef->isVHSWiggleEnabled;
    bufferData->isCurvingEnabled = this->bufferDataRef->isCurvingEnabled;
    bufferData->isChromaticAberrationEnabled =
        this->bufferDataRef->isChromaticAberrationEnabled;
    bufferData->isGrainEnabled = this->bufferDataRef->isGrainEnabled;
    bufferData->isRGBGridEnabled = this->bufferDataRef->isRGBGridEnabled;
    bufferData->isScanlinesEnabled = this->bufferDataRef->isScanlinesEnabled;
    bufferData->isVingetteEnabled = this->bufferDataRef->isVingetteEnabled;

    bufferData->brightnessMultiplier =
        this->bufferDataRef->brightnessMultiplier;
    bufferData->grainStrength = this->bufferDataRef->grainStrength;
    bufferData->curvePower = this->bufferDataRef->curvePower;
    bufferData->vingetteStrength = this->bufferDataRef->vingetteStrength;
    bufferData->scanlinesInterval = this->bufferDataRef->scanlinesInterval;
    bufferData->scanlinesOpacity = this->bufferDataRef->scanlinesOpacity;
    bufferData->scanlinesThickness = this->bufferDataRef->scanlinesThickness;
    bufferData->chromaticAberrationStrength =
        this->bufferDataRef->chromaticAberrationStrength;
    bufferData->vhsWiggle = this->bufferDataRef->vhsWiggle;

    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->PSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->pixelShaderCounters.constantSlot++;
    return true;
}
