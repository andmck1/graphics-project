#pragma once

class BaseTexture
{
    public:
    virtual ~BaseTexture() = default;
    virtual bool load() = 0;
    virtual bool setPixelShader(int startSlot, int numberView) = 0;
};
