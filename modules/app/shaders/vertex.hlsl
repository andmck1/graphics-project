cbuffer MatrixBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT
{
    float3 position : POSITION;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;

    float4 worldPos = mul(float4(input.position, 1.0f), world);
    float4 viewPos  = mul(worldPos, view);
    output.position = mul(viewPos, projection);

    return output;
}


