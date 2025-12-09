#include "sharedbuffertypes.h"

TessellationVertexOutputType main(TessellationVertexInputType input)
{
    // pass through for hull, domain
    TessellationVertexOutputType output;
    
    output.position = input.position;
    output.tex = input.tex;
    output.normal = input.normal;
    
    return output;
}

