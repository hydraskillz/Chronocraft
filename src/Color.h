/*
 * Author      : Matthew Johnson
 * Date        : 13/Nov/2012
 * Description :
 *   Colors!
 */
#pragma once

struct Color
{
	Color()
		: r( 1 ), g( 1 ), b( 1 ), a( 0 )
	{}
	Color( float r, float g, float b, float a=0 )
		: r( r ), g( g ), b( b ), a( a )
	{}

	Color& operator=( const Color& other )
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
		return *this;
	}

	float r, g, b, a;

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color ORANGE;
	static const Color YELLOW;
	static const Color PURPLE;
	static const Color BLACK;
	static const Color WHITE;
};