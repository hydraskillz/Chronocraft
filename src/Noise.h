#pragma once

namespace mzj
{

	namespace Noise
	{
		float Noise1D( int x );
		float SmoothNoise1D( float x );
		float InterpolatedNoise1D( float x );
		float Perlin1D( int x );

		float Noise2D( int x, int y );
		float SmoothNoise2D( float x, float y );
		float InterpolatedNoise2D( float x, float y );
		float Perlin2D( float x, float y );
	}

}