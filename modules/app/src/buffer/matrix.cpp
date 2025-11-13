#include "matrix.h"
#include <core_utils/graphics.h>

bool MatrixVertexBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising matrix buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(MatrixVertexBuffer::ShaderBuffer);
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

void MatrixVertexBuffer::setData(
    XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
    XMMATRIX spotLightViewMatrix1, XMMATRIX spotLightProjectionMatrix1,
    XMMATRIX spotLightViewMatrix2, XMMATRIX spotLightProjectionMatrix2
)
{
    this->worldMatrix = worldMatrix;
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->spotLightViewMatrix1 = spotLightViewMatrix1;
    this->spotLightViewMatrix2 = spotLightViewMatrix2;
    this->spotLightProjectionMatrix1 = spotLightProjectionMatrix1;
    this->spotLightProjectionMatrix2 = spotLightProjectionMatrix2;
}

bool MatrixVertexBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixVertexBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->vertexShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Matrix buffer map failed.\n"))
        return false;
    bufferData = (MatrixVertexBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->projectionMatrix = XMMatrixTranspose(this->projectionMatrix);
    bufferData->worldMatrix = XMMatrixTranspose(this->worldMatrix);
    bufferData->viewMatrix = XMMatrixTranspose(this->viewMatrix);
    bufferData->spotLightViewMatrix1 =
        XMMatrixTranspose(this->spotLightViewMatrix1);
    bufferData->spotLightViewMatrix2 =
        XMMatrixTranspose(this->spotLightViewMatrix2);
    bufferData->spotLightProjectionMatrix1 =
        XMMatrixTranspose(this->spotLightProjectionMatrix1);
    bufferData->spotLightProjectionMatrix2 =
        XMMatrixTranspose(this->spotLightProjectionMatrix2);
    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->VSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->vertexShaderCounters.constantSlot++;
    return true;
}

bool MatrixHullBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising matrix buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(MatrixHullBuffer::ShaderBuffer);
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

void MatrixHullBuffer::setData(
    XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix
)
{
    this->worldMatrix = worldMatrix;
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
}

bool MatrixHullBuffer::setShader(BufferSlots *slotCounters, int numberOfBuffers)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixHullBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->hullShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Matrix buffer map failed.\n"))
        return false;
    bufferData = (MatrixHullBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->projectionMatrix = XMMatrixTranspose(this->projectionMatrix);
    bufferData->worldMatrix = XMMatrixTranspose(this->worldMatrix);
    bufferData->viewMatrix = XMMatrixTranspose(this->viewMatrix);
    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->HSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->hullShaderCounters.constantSlot++;
    return true;
}

bool MatrixDomainBuffer::init(D3D *graphicsManager)
{
    HRESULT createBufferHR;

    spdlog::debug("Initialising matrix buffer");
    this->graphicsManager = graphicsManager;
    this->bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    this->bufferDesc.ByteWidth = sizeof(MatrixDomainBuffer::ShaderBuffer);
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

void MatrixDomainBuffer::setData(
    XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
    XMMATRIX spotLightViewMatrix1, XMMATRIX spotLightProjectionMatrix1,
    XMMATRIX spotLightViewMatrix2, XMMATRIX spotLightProjectionMatrix2
)
{
    this->worldMatrix = worldMatrix;
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->spotLightViewMatrix1 = spotLightViewMatrix1;
    this->spotLightViewMatrix2 = spotLightViewMatrix2;
    this->spotLightProjectionMatrix1 = spotLightProjectionMatrix1;
    this->spotLightProjectionMatrix2 = spotLightProjectionMatrix2;
}

bool MatrixDomainBuffer::setShader(
    BufferSlots *slotCounters, int numberOfBuffers
)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixDomainBuffer::ShaderBuffer *bufferData;
    HRESULT mapHR;

    int bufferSlot = slotCounters->domainShaderCounters.constantSlot;

    mapHR = graphicsManager->deviceContext->Map(
        this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource
    );
    if (!TryDXCall(mapHR, "Matrix buffer map failed.\n"))
        return false;
    bufferData = (MatrixDomainBuffer::ShaderBuffer *) mappedResource.pData;
    bufferData->projectionMatrix = XMMatrixTranspose(this->projectionMatrix);
    bufferData->worldMatrix = XMMatrixTranspose(this->worldMatrix);
    bufferData->viewMatrix = XMMatrixTranspose(this->viewMatrix);
    bufferData->spotLightViewMatrix1 =
        XMMatrixTranspose(this->spotLightViewMatrix1);
    bufferData->spotLightViewMatrix2 =
        XMMatrixTranspose(this->spotLightViewMatrix2);
    bufferData->spotLightProjectionMatrix1 =
        XMMatrixTranspose(this->spotLightProjectionMatrix1);
    bufferData->spotLightProjectionMatrix2 =
        XMMatrixTranspose(this->spotLightProjectionMatrix2);
    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->DSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->domainShaderCounters.constantSlot++;
    return true;
}
