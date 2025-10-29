cbuffer LightBuffer : register(b0)
{
    float4 colour;
    float4 position;
    float4 direction;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 lightColour;

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	lightColour = calculateLighting(-direction.xyz, input.normal, colour);
	
	
    return lightColour;
}
