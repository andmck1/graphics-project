cbuffer LightBuffer : register(b0)
{
    float4 colour;
    float4 position;
}

struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 worldPosition : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET {
    float4 lightVector = position.xyz - input.worldPosition.xyz;
    float d = length(lightVector);
    float attenuation = 1.f / (1.f + 0.01*d);
    return saturate(colour * attenuation);
}


