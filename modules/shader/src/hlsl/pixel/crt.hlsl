#include "sharedbuffertypes.h"

Texture2D objectTexture: register(t0);
SamplerState textureSampler: register(s0);
Texture2D shadowMapTexture: register(t1);
Texture2D shadowMapTexture2: register(t2);

cbuffer CRTBufferType : register(b0)
{
	CRTBufferType crtData;
};

float2 distort(float2 _uv, float curvePower)
{
	// We want to mimic the shape of a tv screen!
	// This means we want to have a lens like distortion on our image.
	// The closer we are to the edge, distort more.
	// Building on Art-Michaels example I really wanted good falloff
	// towards the edge so do a linear interpolation between the distortion
	// and the original coordinates.
	float angle, theta, radius, distanceFromCentre, falloff;
	float2 uvDistorted;

	uvDistorted = _uv * 2.0 - 1.0;
	distanceFromCentre = length(uvDistorted);
	theta = atan2(uvDistorted.y, uvDistorted.x);
	radius = pow(distanceFromCentre, curvePower);
	uvDistorted.x = radius * cos(theta);
	uvDistorted.y = radius * sin(theta);

	uvDistorted = uvDistorted * 0.5 + 0.5; 

	falloff = smoothstep(0.5, 1.0, distanceFromCentre);
	uvDistorted = lerp(_uv, uvDistorted, falloff);

	return saturate(uvDistorted);
}

// Help from Copilot for these random functions
float random(float2 seed)
{
	float2 randomNumber;
	randomNumber = frac(seed * float2(123.33, 672.12));
	randomNumber += dot(randomNumber, randomNumber.yx + 12.93);    
	return frac(randomNumber.x * randomNumber.y);
}

float randomPeriodic( float2 seed, float _totalTime)
{
	return random(seed + float2(_totalTime, _totalTime * 0.21));
}

float4 getTextureColour(Texture2D _objectTexture, SamplerState _textureSampler, float2 uv)
{
	return _objectTexture.Sample(_textureSampler, uv);
}

float vhsWiggleOffset(float2 _uv, float _totalTime, float wiggleAmount)
{
	float wiggleOffset;
	wiggleOffset = (
		randomPeriodic(_uv, _totalTime) - 0.5
		) * wiggleAmount;

	return wiggleOffset;
}

float4 textureWithChromaticAberration(
	Texture2D _texture, SamplerState _sampler, float2 _uv, float _uResolution,
	float chromaticAbberrationStrength
)
{
	// Sample left/right (U direction) and take the r strength steps along negative U
	// and b strength steps along positive U
	float4 chromaticAberrationColour;
	float2 rUVOffset, gUVOffset, bUVOffset;
	rUVOffset = float2(_uResolution * chromaticAbberrationStrength, 0.f);
	gUVOffset = float2(0.f, 0.f);
	bUVOffset = float2(-_uResolution * chromaticAbberrationStrength, 0.f);

	chromaticAberrationColour = float4(
		getTextureColour(_texture, _sampler, _uv + rUVOffset).r,
		getTextureColour(_texture, _sampler, _uv + gUVOffset).g,
		getTextureColour(_texture, _sampler, _uv + bUVOffset).b,
		getTextureColour(_texture, _sampler, _uv).a
	);

	return chromaticAberrationColour;

}

float4 colourWithFilmGrainNoise(float _totalTime, float2 _uv, float4 _colour, float _strength)
{
	float noiseR, noiseG, noiseB;
	float4 newColour;

	noiseR = (random(_uv) * 2.f - 1.f) * _strength;
	noiseG = (random(_uv + float2(39.32f, 8.29f)) * 2.f - 1.f) * _strength;
	noiseB = (random(_uv + float2(2.44f, 38.4f)) * 2.f - 1.f) * _strength;

	newColour.r = _colour.r + noiseR;
	newColour.g = _colour.g + noiseG;
	newColour.b = _colour.b + noiseB;
	newColour.a = _colour.a;

	return saturate(newColour);
}

float4 getRGBGrideMask(float4 _screenSpacePosition) 
{
	// Want to make the first pixel more red, the second more green and the third more blue
	// cyclically across the screen in rows
	float4 rgbMask = float4(0.f, 0.f, 0.f, 1.f);
	uint gridIndex = uint(_screenSpacePosition.x) % 3;
	if (gridIndex == 0) rgbMask.r = 1.f;
	else if (gridIndex == 1) rgbMask.g = 1.f;
	else if (gridIndex == 2) rgbMask.b = 1.f;
	return rgbMask;
}

float calculateScanlineColourModulation(
	float4 _screenSpacePosition, int _scanlineInterval,
	int _scanlineThickness, float _scanlineOpacity
)
{
	// We want to set lines that aren't the scanline to black
	// This means we check to see if the current row is a scanline
	// And then modulate to black if not
	// If scanline interval is 2 and scanline thickness is 3 then we
	// Colour 3 rows and leave 2 black
	float scanlineModulation = 1.f;
	uint scanCycleSize = _scanlineInterval + _scanlineThickness; 
	uint rowNumberInCycle = int(_screenSpacePosition.y) % scanCycleSize;
	scanlineModulation = rowNumberInCycle > (uint) _scanlineThickness ? 0.f : 1.f;
	scanlineModulation *= _scanlineOpacity;
	return scanlineModulation;	
}

// Falloff function from copilot, works well
float calculateVingetteModulation(
	float2 _uv, 
	float _vingetteStrength
)
{
	// A vingette darkens the image the further away it is from the centre.
	// We want the darkening to fall off quickly.
	float vingetteModulation = 1.f;
	float distanceFromCentre = length(_uv - float2(0.5f, 0.5f));
	vingetteModulation = pow(1 - distanceFromCentre, _vingetteStrength);
	return vingetteModulation;
}

float4 main(PixelInputType input) : SV_TARGET
{
	float4 totalColour = float4(0.f, 0.f, 0.f, 1.f);
	float2 uv = input.tex;
	float uResolution, vResolution;

	uint height;
    uint width;
    objectTexture.GetDimensions(width, height);

	uResolution = 1.f / (float) width;
	vResolution = 1.f / (float) height;

	if ((bool) crtData.isVHSWiggleEnabled.x)
	{
		uv.x += vhsWiggleOffset(uv, crtData.totalTime.x, crtData.vhsWiggle.x);
	}

	if ((bool) crtData.isCurvingEnabled.x)
	{
		uv = distort(uv, crtData.curvePower.x);
	}

	totalColour = getTextureColour(objectTexture, textureSampler, uv);

	if ((bool) crtData.isChromaticAberrationEnabled.x)
	{
		totalColour = textureWithChromaticAberration(
			objectTexture, 
			textureSampler, 
			uv,
			uResolution,
			crtData.chromaticAberrationStrength.x
		);
	}

	if ((bool) crtData.isGrainEnabled.x)
	{
		totalColour = colourWithFilmGrainNoise(
			crtData.totalTime.x, uv, totalColour, crtData.grainStrength.x
		);
	}

	if ((bool) crtData.isRGBGridEnabled)
	{
		totalColour *= getRGBGrideMask(input.position);
	}

	totalColour *= crtData.brightnessMultiplier.x;

	if ((bool) crtData.isScanlinesEnabled.x)
	{
		totalColour *= calculateScanlineColourModulation(
			input.position, crtData.scanlinesInterval.x,
			crtData.scanlinesThickness.x, crtData.scanlinesOpacity.x
		);
	}

	if ((bool) crtData.isVingetteEnabled.x)
	{
		totalColour.rgb *= calculateVingetteModulation(
			input.tex, crtData.vingetteStrength.x
		);
	}

	return saturate(totalColour);
}
