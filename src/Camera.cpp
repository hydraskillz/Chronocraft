#include "Camera.h"

using namespace mzj;

Camera::Camera()
	: Actor()
{
	drag = 5.5f;
}

Camera::~Camera()
{}

void Camera::LookAt( const Vec3f& target )
{
	LookAt( target, transform.GetTranslation(),Vec3f( 0, 1, 0 ) );
}

void Camera::LookAt( const Vec3f& eye, const Vec3f& target, const Vec3f& up )
{
	transform = Mat4f::MakeLookAt( eye, target, up );
}

void Camera::SetCameraSettings( float angle, float aspect, float nearD, float farD )
{
	this->angle = angle;
	this->aspect = aspect;
	this->nearD = nearD;
	this->farD = farD;

	tanangle = (float) std::tan( degreesToRadians( angle ) * 0.5 );
	nearH = nearD * tanangle;
	nearW = nearH * aspect;
	farH = farD * tanangle;
	farW = farH * aspect;
}

void Camera::CalculateFrustum()
{
	up.Set( 0, 1, 0 );

	left = Vec3f::Cross( position, up );
	left.Normalize();

	Vec3f dir, nc, fc, x, y, z;

	z = position - left;
	z.Normalize();

	x = Vec3f::Cross( up, z );
	x.Normalize();

	y = Vec3f::Cross( z, x );

	nc = position - z * nearD;
	fc = position - z * farD;

	nearPoints[0] = nc + y * nearH - x * nearW;
	nearPoints[1] = nc + y * nearH + x * nearW;
	nearPoints[2] = nc - y * nearH - x * nearW;
	nearPoints[3] = nc - y * nearH + x * nearW;

	farPoints[0] = fc + y * farH - x * farW;
	farPoints[1] = fc + y * farH + x * farW;
	farPoints[2] = fc - y * farH - x * farW;
	farPoints[3] = fc - y * farH + x * farW;

	planes[TOP].Set( nearPoints[1], nearPoints[0], farPoints[0] );
	planes[BOTTOM].Set( nearPoints[2], nearPoints[3], farPoints[3] );
	planes[LEFT].Set( nearPoints[0], nearPoints[2], farPoints[2] );
	planes[RIGHT].Set( nearPoints[3], nearPoints[1], farPoints[3] );
	planes[NEARP].Set( nearPoints[0], nearPoints[1], nearPoints[3] );
	planes[FARP].Set( farPoints[1], farPoints[0], farPoints[2] );
}


int Camera::PointInFrustum( const Vec3f& p ) const
{
	for ( int i = 0; i < 6; ++i )
	{
		if ( planes[i].DistanceToPoint( p ) < 0 )
			return OUTSIDE;
	}
	return INSIDE;
}

int Camera::CubeInFrustum( const Vec3f& p, float w ) const
{
	return 0;
}