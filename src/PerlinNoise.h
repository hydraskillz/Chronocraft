#pragma once

#include <math.h>

double noise1(double arg);
double noise2(double vec[2]);
double noise3(double vec[3]);

double noise3D( int octaves, double x, double y, double z )
{
	double n = 0.0;
	for ( int i = 0; i < octaves; ++i )
	{
		double v[] = { x * std::powl( 2, i ), y * powl( 2, i ), z * powl( 2, i ) };
		n += noise3( v );
	}
	return n;
}