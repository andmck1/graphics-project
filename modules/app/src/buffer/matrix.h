#pragma once

#include <core_graphics/d3d/d3d.h>
#include <core_graphics/buffer/base.h>
#include <core_graphics/camera/camera.h>

#include "shared_shader/sharedbuffertypes.h"

class MatrixBuffer : public BaseConstantBuffer
{
    public:
    bool init(D3D *graphicsManager) override;
    bool setShader(BufferSlots *slotCounters, int numberOfBuffers) override;
    void setData(
        XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
        XMMATRIX spotLightViewMatrix1 = XMMatrixIdentity(),
        XMMATRIX spotLightProjectionMatrix1 = XMMatrixIdentity(),
        XMMATRIX spotLightViewMatrix2 = XMMatrixIdentity(),
        XMMATRIX spotLightProjectionMatrix2 = XMMatrixIdentity()
    );

    XMMATRIX worldMatrix;
    XMMATRIX viewMatrix;
    XMMATRIX projectionMatrix;
    XMMATRIX spotLightViewMatrix1;
    XMMATRIX spotLightProjectionMatrix1;
    XMMATRIX spotLightViewMatrix2;
    XMMATRIX spotLightProjectionMatrix2;

    struct ShaderBuffer : SharedBufferTypes::MatrixBufferType
    {
    };
};

class MatrixDomainBuffer : public BaseConstantBuffer
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
