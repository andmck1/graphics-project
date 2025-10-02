// Pixel Shader
struct PS_INPUT {
    float4 pos : SV_POSITION;
    float3 worldPos : WORLD_POSITION;
};

float4 main(PS_INPUT input) : SV_TARGET {
    return float4(1.0, 0.0, 0.0, 1.0);
}

