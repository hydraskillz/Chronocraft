/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   Collection of math utility functions and classes.
 */
#pragma once

#include <cstdlib>

/* Math Utils
 * 
 * Functions:
 *   degreesToRadians( degrees )
 *   radiansToDegrees( radians )
 *   randInRange( min, max )
 */

const double PI = 3.141592654;
const double piover180 = PI / 180.0;
const double randMax = static_cast< double >( RAND_MAX );

template< class T >
inline T degreesToRadians( T degrees )
{
	return static_cast< T >( degrees * piover180 );
}

template< class T >
inline T radiansToDegrees( T radians )
{
	return static_cast< T >( radians / PI * 180.0 );
}

template< class T >
inline T randInRange( T min, T max )
{
	return min + static_cast< T >( ( max - min ) * ( rand() / randMax ) );
}

template< class T >
T clamp( T val, T min, T max )
{
	return val < min ? min : val > max ? max : val;
}

// Matrix uses funtions declared above
#include "Matrix.h"	// Matrix includes Vector

typedef Vector3< float    > Vec3f;
typedef Vector3< double   > Vec3d;
typedef Vector3< int      > Vec3i;
typedef Vector3< unsigned > Vec3u;

typedef Vector2< float    > Vec2f;
typedef Vector2< double   > Vec2d;
typedef Vector2< int      > Vec2i;
typedef Vector2< unsigned > Vec2u;

typedef Matrix4< float    > Mat4f;
typedef Matrix4< double   > Mat4d;
typedef Matrix4< int      > Mat4i;
typedef Matrix4< unsigned > Mat4u;