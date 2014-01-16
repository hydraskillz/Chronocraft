#include "Noise.h"
#include <math.h>

using namespace mzj;

__pragma( warning(push) );
__pragma( warning(disable: 4244) );


float Interpolate( float v1, float v2, float frac )
{
	float ft = frac * 3.11415927;
	float f = ( 1 - cosf( ft ) ) * 0.5f;
	return v1 * ( 1 - f ) + v2 * f;
}

float Noise::Noise1D( int x )
{
	x = ( x << 13 ) ^ x;
	return ( 1.0f - (( x * ( x * x * 15731 + 789221 ) + 1376312589 ) & 0x7fffffff ) / 1073741824.0f );
}

float Noise::SmoothNoise1D( float x )
{
	return Noise1D( x ) / 2 + Noise1D( x - 1 ) / 4 + Noise1D( x + 1 ) / 4;
}

float Noise::InterpolatedNoise1D( float x )
{
	int _x = (int) x;
	float frac = x - _x;

	float v1 = SmoothNoise1D( _x );
	float v2 = SmoothNoise1D( _x + 1 );

	return Interpolate( v1, v2, frac );
}

float Noise::Perlin1D( int x )
{
	float total = 0;
	float p = 0.25f;
	int n = 3;

	for ( int i = 0; i < n; ++i )
	{
		float freq = powf( 2.0f, i );
		float amp = pow( p, i );

		total = total + InterpolatedNoise1D( x * freq ) * amp;
	}

	return total;
}

float Noise::Noise2D( int x, int y )
{
	int n = x + y * 57;
	n = (n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float Noise::SmoothNoise2D( float x, float y )
{
	float corners = ( Noise2D(x-1, y-1)+Noise2D(x+1, y-1)+Noise2D(x-1, y+1)+Noise2D(x+1, y+1) ) / 16;
    float sides   = ( Noise2D(x-1, y)  +Noise2D(x+1, y)  +Noise2D(x, y-1)  +Noise2D(x, y+1) ) /  8;
    float center  =  Noise2D(x, y) / 4;
    return corners + sides + center;
}

float Noise::InterpolatedNoise2D( float x, float y )
{
	int _x = int(x);
    float fracX = x - _x;

    int _y    = int(y);
    float fracY = y - _y;

    float v1 = SmoothNoise2D(_x,     _y);
    float v2 = SmoothNoise2D(_x + 1, _y);
    float v3 = SmoothNoise2D(_x,     _y + 1);
    float v4 = SmoothNoise2D(_x + 1, _y + 1);

    float  i1 = Interpolate(v1 , v2 , fracX);
    float i2 = Interpolate(v3 , v4 , fracX);

    return Interpolate(i1 , i2 , fracY);
}

float Noise::Perlin2D( float x, float y )
{
	float total = 0;
    float p = 0.25f;
    int n = 2;

    for ( int i = 0; i < n; ++i )
	{
        float frequency = pow( 2.0f, i );
        float amplitude = pow( p, i );

        total = total + InterpolatedNoise2D(x * frequency, y * frequency) * amplitude;
	}

    return total;
}

__pragma( warning(pop) );