#pragma once

#include "../d3d/d3d.h"

struct BufferSlots
{
    struct BaseBufferSlotCounters
    {
        int textureSlot = 0;
        int samplerSlot = 0;
        int constantSlot = 0;
    };

    struct VertexShaderCounters : BaseBufferSlotCounters
    {
    };

    struct PixelShaderCounters : BaseBufferSlotCounters
    {
    };

    VertexShaderCounters vertexShaderCounters = VertexShaderCounters();
    PixelShaderCounters pixelShaderCounters = PixelShaderCounters();
};

class BaseBuffer
{
    public:
    virtual ~BaseBuffer() = default;
    virtual bool init(D3D *graphicsManager) = 0;
    virtual bool setShader(BufferSlots *slotCounters, int numberOfBuffers) = 0;
    ComPtr<ID3D11Buffer> buffer;
    D3D11_BUFFER_DESC bufferDesc;

    protected:
    D3D *graphicsManager;
};

class BaseTextureBuffer : public BaseBuffer
{
};

class BaseConstantBuffer : public BaseBuffer
{
};

class BaseStructuredBuffer : public BaseTextureBuffer
{
    public:
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ComPtr<ID3D11ShaderResourceView> shaderResourceView;
};
