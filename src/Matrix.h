/*
 * Author      : Matthew Johnson
 * Date        : 20/Oct/2012
 * Description :
 *   Feature rich 4x4 Matrix.
 */
#pragma once

#include "Vector.h"

#include <memory.h>
//#include <stdarg.h>

template< class T >
class Matrix4
{
public:
	Matrix4() { Identity(); }
	Matrix4( T* arr ) { memcpy( mat, arr, 16 * sizeof( T ) ); }
	/*
	Matrix4( T n, ... )
	{
		va_list ap;
		int i = 0;
		va_start( ap, n );
		for (; n; n-- )
			mat[i++] = va_arg(ap, T );
		va_end(ap);
	}
	*/
	Matrix4( T a, T b, T c, T d,
		     T e, T f, T g, T h,
			 T i, T j, T k, T l,
			 T m, T n, T o, T p )
	{
		mat[0] = a; mat[1] = b; mat[2] = c; mat[3] = d;
		mat[4] = e; mat[5] = f; mat[6] = g; mat[7] = h;
		mat[8] = i; mat[9] = j; mat[10]= k; mat[11]= l;
		mat[12]= m; mat[13]= n; mat[14]= o; mat[15]= p;
	}
	~Matrix4() {}

	const T* GetArray() const { return mat; }

	const T operator[]( int i ) const
	{
		return mat[i];
	}

	Matrix4& operator=(const Matrix4& other )
	{
		memcpy( mat, other.mat, 16 * sizeof( T ) );
		return *this;
	}

	Matrix4& Set( const Matrix4& other )
	{
		memcpy( mat, other.mat, 16 * sizeof( T ) );
		return *this;
	}

	Matrix4& Set( const T* arr )
	{
		memcpy( mat, arr, 16 * sizeof( T ) );
		return *this;
	}

	Matrix4 operator-( const Matrix4& other ) const
	{
		Matrix4 m;
		for ( int i = 0; i < 16; ++i )
			m.mat[i] = mat[i] - other.mat[i];
		return m;
	}

	Matrix4& operator-=( const Matrix4& other )
	{
		for ( int i = 0; i < 16; ++i ) mat[i] -= other.mat[i];
		return *this;
	}

	Matrix4 operator+( const Matrix4& other ) const
	{
		Matrix4 m;
		for ( int i = 0; i < 16; ++i )
			m.mat[i] = mat[i] + other.mat[i];
		return m;
	}

	Matrix4& operator+=( const Matrix4& other )
	{
		for ( int i = 0; i < 16; ++i )
			mat[i] += other.mat[i];
		return *this;
	}

	Matrix4 operator*( const Matrix4& other ) const
	{
		Matrix4 m;
		m.mat[0] = mat[0]*other.mat[0] + mat[1]*other.mat[4] + mat[2]*other.mat[8] + mat[3]*other.mat[12];
		m.mat[1] = mat[0]*other.mat[1] + mat[1]*other.mat[5] + mat[2]*other.mat[9] + mat[3]*other.mat[13];
		m.mat[2] = mat[0]*other.mat[2] + mat[1]*other.mat[6] + mat[2]*other.mat[10] + mat[3]*other.mat[14];
		m.mat[3] = mat[0]*other.mat[3] + mat[1]*other.mat[7] + mat[2]*other.mat[11] + mat[3]*other.mat[15];

		m.mat[4] = mat[4]*other.mat[0] + mat[5]*other.mat[4] + mat[6]*other.mat[8] + mat[7]*other.mat[12];
		m.mat[5] = mat[4]*other.mat[1] + mat[5]*other.mat[5] + mat[6]*other.mat[9] + mat[7]*other.mat[13];
		m.mat[6] = mat[4]*other.mat[2] + mat[5]*other.mat[6] + mat[6]*other.mat[10] + mat[7]*other.mat[14];
		m.mat[7] = mat[4]*other.mat[3] + mat[5]*other.mat[7] + mat[6]*other.mat[11] + mat[7]*other.mat[15];

		m.mat[8] = mat[8]*other.mat[0] + mat[9]*other.mat[4] + mat[10]*other.mat[8] + mat[11]*other.mat[12];
		m.mat[9] = mat[8]*other.mat[1] + mat[9]*other.mat[5] + mat[10]*other.mat[9] + mat[11]*other.mat[13];
		m.mat[10] = mat[8]*other.mat[2] + mat[9]*other.mat[6] + mat[10]*other.mat[10] + mat[11]*other.mat[14];
		m.mat[11] = mat[8]*other.mat[3] + mat[9]*other.mat[7] + mat[10]*other.mat[11] + mat[11]*other.mat[15];

		m.mat[12] = mat[12]*other.mat[0] + mat[13]*other.mat[4] + mat[14]*other.mat[8] + mat[15]*other.mat[12];
		m.mat[13] = mat[12]*other.mat[1] + mat[13]*other.mat[5] + mat[14]*other.mat[9] + mat[15]*other.mat[13];
		m.mat[14] = mat[12]*other.mat[2] + mat[13]*other.mat[6] + mat[14]*other.mat[10] + mat[15]*other.mat[14];
		m.mat[15] = mat[12]*other.mat[3] + mat[13]*other.mat[7] + mat[14]*other.mat[11] + mat[15]*other.mat[15];
		/*
		for ( int i = 0; i < 16; ++i )
		{
			int j = i%4, u = i/4 * 4;
			for ( int k = u; k < u + 4; ++k )
				m.mat[i++] += mat[k] * other.mat[j+4*k];
		}
		*/
		return m;
	}

	Matrix4& operator*=( const Matrix4& other )
	{
		Matrix tmp;
		for ( int i = 0; i < 16; ++i )
		{
			int j = i%4, u = i/4 * 4;
			for ( int k = u; k < u + 4; ++k )
				tmp.mat[i++] += mat[j+4*k] * other.mat[k];
		}
		return ( *this = tmp );
	}

	Matrix4 operator*( const T scalar ) const
	{
		Matrix4 m;
		for ( int i = 0; i < 16; ++i )
			m.mat[i] = mat[i] * scalar;
		return m;
	}

	Matrix4& operator*=( const T scalar )
	{
		for ( int i = 0; i < 16; ++i )
			mat[i] *= scalar;
	}

	void Identity()
	{
		for ( int i = 0; i < 16; ++i )
			mat[i] = static_cast< T >( ( i % 5 ) ? 0 : 1 );
	}

	void Inverse()
	{
		// TODO
		mat[12] = -mat[12];
		mat[13]	= -mat[13];
		mat[14]	= -mat[14];
	}

	Matrix4& Transpose()
	{
		//in-place transpose
		std::swap(mat[1],mat[4]);
		std::swap(mat[2],mat[8]);
		std::swap(mat[3],mat[12]);
		std::swap(mat[6],mat[9]);
		std::swap(mat[7],mat[13]);
		std::swap(mat[11],mat[14]);
		/*
		for ( int i = 0; i < 16; ++i )
		{
			int u = i / 4;
			for ( int k = u; k <= u + 12; k += 4 )
				mat[i++] = mat[k];
		}*/
		return *this;
	}

	static Matrix4 Transpose( const Matrix4& mat )
	{
		Matrix4 m = mat;
		m.Transpose();
		return m;
	}

	static Matrix4 Inverse( const Matrix4& mat )
	{
		Matrix4 m = mat;
		m.Inverse();
		return m;
	}

	void Translate( const Vector3< T >& trans )
	{
		Identity();
		mat[12] = trans.x;
		mat[13] = trans.y;
		mat[14] = trans.z;
	}

	Vector3< T > GetTranslation()
	{
		return Vector3< T >( mat[12], mat[13], mat[14] );
	}

	// Axis needs to be normalized
	void Rotate( T angle, const Vector3< T >& axis )
	{
		T s = static_cast< T >( std::sin( degreesToRadians( angle ) ) );
		T c = static_cast< T >( std::cos( degreesToRadians( angle ) ) );

		T ux = axis.x;
		T uy = axis.y;
		T uz = axis.z;

		// Axis Angle rotation matrix: http://en.wikipedia.org/wiki/Rotation_matrix
		mat[0]  = c + ux*ux*(1-c);
		mat[1]  = uy*ux*(1-c) + uz*s;
		mat[2]  = uz*ux*(1-c) - uy*s;
		mat[3]  = 0;
    
		mat[4]  = ux*uy*(1-c) - uz*s;
		mat[5]  = c + uy*uy*(1-c);
		mat[6]  = uz*uy*(1-c) + ux*s;
		mat[7]  = 0;
    
		mat[8]  = ux*uz*(1-c) + uy*s;
		mat[9]  = uy*uz*(1-c) - ux*s;
		mat[10] = c + uz*uz*(1-c);
		mat[11] = 0;
    
		mat[12] = 0;
		mat[13] = 0;
		mat[14] = 0;
		mat[15] = 1;
	}

	static Matrix4 MakeLookAt( const Vector3< T >& eye, const Vector3< T >& center, const Vector3< T > up )
	{
		Vector3< T > Kc = center - eye;
		Kc.Normalize();

		Vector3< T > Ic = Vector3< T >::Cross( up, Kc );
		Ic.Normalize();

		Vector3< T > Jc = Vector3< T >::Cross( Kc, Ic );

		Matrix4 R(
			Ic.x, Ic.y, Ic.z, 0,
			Jc.x, Jc.y, Jc.z, 0,
			Kc.x, Kc.y, Kc.z, 0,
			0, 0, 0, 1 );

		R.Transpose();

		Matrix4 Tr;
		Tr.Translate( -center );

		return Tr * R;
	}

private:
	T mat[16];	// tvou mat
};