#pragma once

#include "../d3d/d3d.h"

class Buffer
{
    public:
    virtual ~Buffer() = default;
    virtual bool init(D3D *graphicsManager) = 0;
    ComPtr<ID3D11Buffer> buffer;
    D3D11_BUFFER_DESC bufferDesc;

    protected:
    D3D *graphicsManager;
};

class VertexBuffer : public Buffer
{
    public:
    virtual bool setVertexShader(int bufferSlot, int numberOfBuffers) = 0;
};

class PixelBuffer : public Buffer
{
    public:
    virtual bool setPixelShader(int bufferSlot, int numberOfBuffers) = 0;
};

class StructuredBuffer : public Buffer
{
};

class StructuredPixelBuffer : public StructuredBuffer
{
    public:
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    virtual bool setPixelShader(int bufferSlot, int numberOfBuffers) = 0;
};
