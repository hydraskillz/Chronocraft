/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   A camera.
 * TODO:
 *   Test LookAt functions to verify if they work or not.
 */
#pragma once

#include "Actor.h"
#include "Plane.h"

namespace mzj
{

	class Camera
		: public Actor
	{
	public:
		Camera();
		~Camera();

		// Look at a specific point
		void LookAt( const Vec3f& eye );
		void LookAt( const Vec3f& eye, const Vec3f& center, const Vec3f& up );

		void SetCameraSettings( float angle, float aspect, float nearD, float farD );
		void CalculateFrustum();

		int PointInFrustum( const Vec3f& p ) const;
		int CubeInFrustum( const Vec3f& p, float w ) const;

		enum
		{
			INSIDE,
			OUTSIDE,
			INTERSECT
		};

	private:
		enum CPLANES
		{
			TOP = 0,
			BOTTOM,
			LEFT,
			RIGHT,
			NEARP,
			FARP
		};
		Plane planes[6];

		float nearD, nearW, nearH;
		float farD, farW, farH;
		float tanangle;

		Vec3f nearPoints[4]; // top-left, top-right, bottom-left, bottom-right
		Vec3f farPoints[4];  // top-left, top-right, bottom-left, bottom-right

		float angle;
		float aspect;

		Vec3f up, left;
	};

}