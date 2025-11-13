#pragma once

#include "core_graphics/shader/shader.h"

// Vertex
class StandardVertexShader : public VertexShader
{
};

class HeightMapVertexShader : public VertexShader
{
};

class WaveVertexShader : public VertexShader
{
};

class DepthVertexShader : public VertexShader
{
};

class TessellationPassThroughVertexShader : public VertexShader
{
};

// Domain
class TessellatedTriangleDomainShader : public DomainShader
{
};

class TessellatedQuadDomainShader : public DomainShader
{
};

// Hull
class TessellatedTriangleHullShader : public HullShader
{
};

class TessellatedQuadHullShader : public HullShader
{
};

// Pixel
class LightingPixelShader : public PixelShader
{
};

class TextureOnlyPixelShader : public PixelShader
{
};

class CRTPixelShader : public PixelShader
{
};

class DepthPixelShader : public PixelShader
{
};
