#include "../graphics/utils.h"
#include "scene.h"

TriangleMesh::TriangleMesh(D3D *graphicsManager)
{
    this->init();
    this->createBuffers(graphicsManager);
}

int TriangleMesh::getVertexCount()
{
    return this->vertexCoordinates.size();
}

int TriangleMesh::getVertexIndexCount()
{
    return this->vertexIndices.size();
}

void TriangleMesh::init()
{
    this->vertexCoordinates.push_back({0.0f, 1.0f, 0.0f});
    this->vertexIndices.push_back(0);
    this->vertexCoordinates.push_back({-1.0f, 0.0f, 0.0f});
    this->vertexIndices.push_back(1);
    this->vertexCoordinates.push_back({1.0f, 0.0f, 0.0f});
    this->vertexIndices.push_back(2);
}

bool TriangleMesh::createBuffers(D3D *graphicsManager)
{
    HRESULT createBufferHR;
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    D3D11_BUFFER_DESC indexBufferDesc = {};
    D3D11_SUBRESOURCE_DATA vertexBufferData;
    D3D11_SUBRESOURCE_DATA indexBufferData;

    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * this->getVertexCount();

    vertexBufferData.pSysMem = this->vertexCoordinates.data();
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    createBufferHR = graphicsManager->device->CreateBuffer(
        &vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create vertex buffer.\n"))
    {
        return false;
    }

    indexBufferDesc.ByteWidth =
        sizeof(unsigned long) * this->getVertexIndexCount();
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexBufferData.pSysMem = this->vertexIndices.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    createBufferHR = graphicsManager->device->CreateBuffer(
        &indexBufferDesc, &indexBufferData,
        this->vertexIndexBuffer.GetAddressOf()
    );
    if (!TryDXCall(createBufferHR, "Failed to create vertex buffer.\n"))
    {
        return false;
    }

    return true;
}

Camera::Camera()
{
    XMMATRIX cameraRotation = getCameraRotation();
    XMVECTOR forward =
        XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), cameraRotation);
    XMVECTOR up =
        XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), cameraRotation);
    XMVECTOR at = XMVectorAdd(this->eyePosition, forward);

    this->viewMatrix = XMMatrixLookAtLH(this->eyePosition, at, up);
}

XMMATRIX Camera::getCameraRotation()
{
    XMMATRIX yawMatrix = XMMatrixRotationY(yaw);
    XMMATRIX pitchMatrix = XMMatrixRotationX(pitch);
    XMMATRIX cameraRotation = pitchMatrix * yawMatrix;
    return cameraRotation;
}

Scene::Scene(D3D *graphicsManager)
{
    this->graphicsManager = graphicsManager;
}

bool Scene::clearScreen(const float clearColour[4])
{
    this->graphicsManager->deviceContext->ClearRenderTargetView(
        this->graphicsManager->renderTargetView.Get(), clearColour
    );
    return true;
}

bool Scene::presentScreen()
{
    HRESULT presentHR;
    presentHR = this->graphicsManager->swapChain->Present(1, 0);

    return TryDXCall(presentHR, "Present failed.\n");
}

bool Scene::drawScreen()
{
    this->graphicsManager->updateMatrixBuffer(
        this->meshWorldMatrix, this->camera->viewMatrix
    );

    UINT stride = sizeof(XMFLOAT3);
    UINT offset = 0;

    this->graphicsManager->deviceContext->OMSetRenderTargets(
        1, this->graphicsManager->renderTargetView.GetAddressOf(), nullptr
    );
    this->graphicsManager->deviceContext->IASetInputLayout(
        this->graphicsManager->inputLayout.Get()
    );
    this->graphicsManager->deviceContext->IASetVertexBuffers(
        0, 1, this->mesh->vertexBuffer.GetAddressOf(), &stride, &offset
    );
    this->graphicsManager->deviceContext->IASetIndexBuffer(
        this->mesh->vertexIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0
    );
    this->graphicsManager->deviceContext->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
    );
    this->graphicsManager->deviceContext->VSSetShader(
        this->graphicsManager->vertexShader.Get(), nullptr, 0
    );
    this->graphicsManager->deviceContext->VSSetConstantBuffers(
        0, 1, this->graphicsManager->matrixBuffer.GetAddressOf()
    );
    this->graphicsManager->deviceContext->PSSetShader(
        this->graphicsManager->pixelShader.Get(), nullptr, 0
    );
    this->graphicsManager->deviceContext->DrawIndexed(
        this->mesh->getVertexIndexCount(), 0, 0
    );

    return true;
}

bool Scene::render()
{
    const float clearColour[4] = {0.392f, 0.584f, 0.929f, 1.0f};
    if (!this->clearScreen(clearColour))
    {
        return false;
    }

    if (!this->drawScreen())
    {
        return false;
    }

    if (!this->presentScreen())
    {
        return false;
    }

    return true;
}
