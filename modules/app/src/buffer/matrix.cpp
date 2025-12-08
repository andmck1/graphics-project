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
    XMMATRIX spotLightViewMatrix0, XMMATRIX spotLightProjectionMatrix0,
    XMMATRIX spotLightViewMatrix1, XMMATRIX spotLightProjectionMatrix1,
    XMMATRIX directionalLightViewMatrix0,
    XMMATRIX directionalLightProjectionMatrix0,
    XMMATRIX directionalLightViewMatrix1,
    XMMATRIX directionalLightProjectionMatrix1
)
{
    this->worldMatrix = worldMatrix;
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->spotLightViewMatrix0 = spotLightViewMatrix0;
    this->spotLightViewMatrix1 = spotLightViewMatrix1;
    this->spotLightProjectionMatrix0 = spotLightProjectionMatrix0;
    this->spotLightProjectionMatrix1 = spotLightProjectionMatrix1;
    this->directionalLightViewMatrix0 = spotLightViewMatrix0;
    this->directionalLightViewMatrix1 = spotLightViewMatrix1;
    this->directionalLightProjectionMatrix0 = spotLightProjectionMatrix0;
    this->directionalLightProjectionMatrix1 = spotLightProjectionMatrix1;
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
    bufferData->spotLightViewMatrix1 =
        XMMatrixTranspose(this->spotLightViewMatrix1);
    bufferData->spotLightProjectionMatrix0 =
        XMMatrixTranspose(this->spotLightProjectionMatrix0);
    bufferData->spotLightProjectionMatrix1 =
        XMMatrixTranspose(this->spotLightProjectionMatrix1);
    bufferData->directionalLightViewMatrix0 =
        XMMatrixTranspose(this->directionalLightViewMatrix0);
    bufferData->directionalLightViewMatrix1 =
        XMMatrixTranspose(this->directionalLightViewMatrix1);
    bufferData->directionalLightProjectionMatrix0 =
        XMMatrixTranspose(this->directionalLightProjectionMatrix0);
    bufferData->directionalLightProjectionMatrix1 =
        XMMatrixTranspose(this->directionalLightProjectionMatrix1);
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
    XMMATRIX spotLightViewMatrix0, XMMATRIX spotLightProjectionMatrix0,
    XMMATRIX spotLightViewMatrix1, XMMATRIX spotLightProjectionMatrix1,
    XMMATRIX directionalLightViewMatrix0,
    XMMATRIX directionalLightProjectionMatrix0,
    XMMATRIX directionalLightViewMatrix1,
    XMMATRIX directionalLightProjectionMatrix1
)
{
    this->worldMatrix = worldMatrix;
    this->viewMatrix = viewMatrix;
    this->projectionMatrix = projectionMatrix;
    this->spotLightViewMatrix0 = spotLightViewMatrix0;
    this->spotLightViewMatrix1 = spotLightViewMatrix1;
    this->spotLightProjectionMatrix0 = spotLightProjectionMatrix0;
    this->spotLightProjectionMatrix1 = spotLightProjectionMatrix1;
    this->directionalLightViewMatrix0 = directionalLightViewMatrix0;
    this->directionalLightViewMatrix1 = directionalLightViewMatrix1;
    this->directionalLightProjectionMatrix0 = directionalLightProjectionMatrix0;
    this->directionalLightProjectionMatrix1 = directionalLightProjectionMatrix1;
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
    bufferData->spotLightViewMatrix0 =
        XMMatrixTranspose(this->spotLightViewMatrix0);
    bufferData->spotLightViewMatrix1 =
        XMMatrixTranspose(this->spotLightViewMatrix1);
    bufferData->spotLightProjectionMatrix0 =
        XMMatrixTranspose(this->spotLightProjectionMatrix0);
    bufferData->spotLightProjectionMatrix1 =
        XMMatrixTranspose(this->spotLightProjectionMatrix1);
    bufferData->directionalLightViewMatrix0 =
        XMMatrixTranspose(this->directionalLightViewMatrix0);
    bufferData->directionalLightViewMatrix1 =
        XMMatrixTranspose(this->directionalLightViewMatrix1);
    bufferData->directionalLightProjectionMatrix0 =
        XMMatrixTranspose(this->directionalLightProjectionMatrix0);
    bufferData->directionalLightProjectionMatrix1 =
        XMMatrixTranspose(this->directionalLightProjectionMatrix1);
    graphicsManager->deviceContext->Unmap(this->buffer.Get(), 0);
    graphicsManager->deviceContext->DSSetConstantBuffers(
        bufferSlot, numberOfBuffers, this->buffer.GetAddressOf()
    );

    slotCounters->domainShaderCounters.constantSlot++;
    return true;
}
