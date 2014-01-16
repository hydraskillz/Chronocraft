/*
 * Author      : Matthew Johnson
 * Date        : 13/Nov/2012
 * Description :
 *   A plane. Not to be confused with a plain.
 */
#pragma once

#include "math_util.h"

namespace mzj
{

	class Plane
	{
	public:
		Plane(){}
		Plane( const Vec3f& v1, const Vec3f& v2, const Vec3f& v3 )
		{
			Set( v1, v2, v3 );
		}
		~Plane(){}

		void Set( const Vec3f& v1, const Vec3f& v2, const Vec3f& v3 )
		{
			Vec3f a, b;
			a = v1 - v2;
			b = v3 - v2;

			norm = Vec3f::Cross( b, a );
			norm.Normalize();

			p = v2;
			d = -( norm * p );
		}

		void Set( const Vec3f& normal, const Vec3f& point )
		{
			norm = normal;
			norm.Normalize();
			p = point;
			d = -( norm * p );
		}

		void Set( float a, float b, float c, float d )
		{
			norm.Set( a, b, c );
			float len = norm.Length();
			this->d = d / len;
			norm.Set( a/len, b/len, c/len ); // Faster than calling Normalize()
		}

		float DistanceToPoint( const Vec3f& p ) const
		{
			return d + norm * p;
		}

	private:
		Vec3f norm, p;
		float d;
	};

}