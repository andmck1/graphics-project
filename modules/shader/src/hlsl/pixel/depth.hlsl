#include "sharedbuffertypes.h"

// (Robertson, n.d.)
// (Hughes, n.d.)
// (Falconer, n.d.)
float4 main(DepthPixelInputType input) : SV_TARGET
{
	float depthValue;
	depthValue = input.depthPosition.z / input.depthPosition.w;
    return float4(depthValue, depthValue, depthValue, 1.f);
}
