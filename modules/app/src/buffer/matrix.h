#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/camera/camera.h>

#include "shared_shader/sharedbuffertypes.h"

class MatrixVertexBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
        XMMATRIX spotLightViewMatrix0 = XMMatrixIdentity(),
        XMMATRIX spotLightProjectionMatrix0 = XMMatrixIdentity(),
        XMMATRIX spotLightViewMatrix1 = XMMatrixIdentity(),
        XMMATRIX spotLightProjectionMatrix1 = XMMatrixIdentity(),
        XMMATRIX directionalLightViewMatrix0 = XMMatrixIdentity(),
        XMMATRIX directionalLightProjectionMatrix0 = XMMatrixIdentity(),
        XMMATRIX directionalLightViewMatrix1 = XMMatrixIdentity(),
        XMMATRIX directionalLightProjectionMatrix1 = XMMatrixIdentity()
    );

    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;
    XMMATRIX spotLightViewMatrix0;
    XMMATRIX spotLightProjectionMatrix0;
    XMMATRIX spotLightViewMatrix1;
    XMMATRIX spotLightProjectionMatrix1;
    XMMATRIX directionalLightViewMatrix0;
    XMMATRIX directionalLightProjectionMatrix0;
    XMMATRIX directionalLightViewMatrix1;
    XMMATRIX directionalLightProjectionMatrix1;

    struct ShaderBuffer : SharedBufferTypes::MatrixBufferType
    {
    };
};

class MatrixHullBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix
    );

    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;

    struct ShaderBuffer : SharedBufferTypes::MatrixDomainBufferType
    {
    };
};

class MatrixDomainBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
        XMMATRIX spotLightViewMatrix0 = XMMatrixIdentity(),
        XMMATRIX spotLightProjectionMatrix0 = XMMatrixIdentity(),
        XMMATRIX spotLightViewMatrix1 = XMMatrixIdentity(),
        XMMATRIX spotLightProjectionMatrix1 = XMMatrixIdentity(),
        XMMATRIX directionalLightViewMatrix0 = XMMatrixIdentity(),
        XMMATRIX directionalLightProjectionMatrix0 = XMMatrixIdentity(),
        XMMATRIX directionalLightViewMatrix1 = XMMatrixIdentity(),
        XMMATRIX directionalLightProjectionMatrix1 = XMMatrixIdentity()
    );

    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;
    XMMATRIX spotLightViewMatrix0;
    XMMATRIX spotLightProjectionMatrix0;
    XMMATRIX spotLightViewMatrix1;
    XMMATRIX spotLightProjectionMatrix1;
    XMMATRIX directionalLightViewMatrix0;
    XMMATRIX directionalLightProjectionMatrix0;
    XMMATRIX directionalLightViewMatrix1;
    XMMATRIX directionalLightProjectionMatrix1;

    struct ShaderBuffer : SharedBufferTypes::MatrixDomainBufferType
    {
    };
};
