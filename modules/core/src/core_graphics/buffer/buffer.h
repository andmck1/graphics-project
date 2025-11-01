#include "../d3d/d3d.h"

class Buffer
{
    public:
    virtual bool init(D3D *graphicsManager) = 0;
    virtual bool setPixelShader() = 0;
    ComPtr<ID3D11Buffer> buffer;
    D3D11_BUFFER_DESC bufferDesc;

    protected:
    D3D *graphicsManager;
};
