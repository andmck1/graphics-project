// Tessellation Hull Shader
// Prepares control points for tessellation
#include "sharedbuffertypes.h"

cbuffer MatrixBuffer : register(b0)
{
    MatrixHullBufferType matrixData;
};

cbuffer CameraBuffer : register(b1)
{
	CameraHullBufferType cameraData;
};

cbuffer SettingsBuffer : register(b2)
{
    TessellationHullDataBufferType settingsData;
}

QuadTessellationConstantOutputType PatchConstantFunction(
    InputPatch<QuadTessellationHullInputType, 4> inputPatch, 
    uint patchId : SV_PrimitiveID
)
{    
    QuadTessellationConstantOutputType output;
    float tessellationFactor, distanceToCamera;
    float3 centre;
    float4 centreWorldPosition;
    
    const float minTessellationFactor = settingsData.minTessellationFactor.x;
    const float maxTessellationFactor = settingsData.maxTessellationFactor.x;
    const float maxTessellationDistanceThreshold = settingsData.minTessellationDistanceThreshold.x;
    const float minTessellationDistanceThreshold = settingsData.maxTessellationDistanceThreshold.x;

    // Tessellate based on distance from camera
    // Frank Luna help
    // Centre is average of all positions
    centre = 0.25f * (
        inputPatch[0].position + 
        inputPatch[1].position + 
        inputPatch[2].position + 
        inputPatch[3].position
    );
    centreWorldPosition = mul(float4(centre, 1.f), matrixData.worldMatrix);
    distanceToCamera = distance(centreWorldPosition, cameraData.worldPosition);

    // Decreasing distance to Camera increases the tessellation factor up to maximum
    // Altered Frank Luna's example so it doesn't just clip things that are too far away
    // (tessellation factor = 0)
    tessellationFactor = maxTessellationFactor * (
        saturate(
            (minTessellationDistanceThreshold - distanceToCamera ) / 
            (minTessellationDistanceThreshold - maxTessellationDistanceThreshold)
        )
    );
    tessellationFactor = max(minTessellationFactor, tessellationFactor);

	output.edges[0] = tessellationFactor;
	output.edges[1] = tessellationFactor;
	output.edges[2] = tessellationFactor;
	output.edges[3] = tessellationFactor;

	output.inside[0] = tessellationFactor;
	output.inside[1] = tessellationFactor;

    return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
QuadTessellationHullOutputType main(
    InputPatch<QuadTessellationHullInputType, 4> patch, 
    uint pointId : SV_OutputControlPointID, 
    uint patchId : SV_PrimitiveID
)
{
    QuadTessellationHullOutputType output;
    output.position = patch[pointId].position;
    output.tex = patch[pointId].tex;
    output.normal = patch[pointId].normal;
    return output;
}
