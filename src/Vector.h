/*
 * Author      : Matthew Johnson
 * Date        : 15/Oct/2012
 * Description :
 *   Feature rich Vector class.
 */
#pragma once

//////////////////////////////////////////
// Vector 3
//////////////////////////////////////////
template< class T >
class Vector3
{
public:
	// Components
	T x, y, z;

	//////////////////////////////////////////
	// Constructors
	//////////////////////////////////////////

	// Initialize all components
	explicit Vector3( T init=0 )
		: x( init ), y( init ), z( init )
	{}

	// Set components
	Vector3( T x, T y, T z )
		: x( x ), y( y ), z( z )
	{}

	// From array
	explicit Vector3( T* a )
		: x( a[0] ), y( a[1] ), z( a[2] )
	{}

	// Copy
	Vector3( const Vector3& other )
		: x( other.x ), y( other.y ), z( other.z )
	{}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Assignment
	//////////////////////////////////////////
	Vector3& operator=( const Vector3& other )
	{
		Set( other );
		return *this;
	}

	Vector3& operator=( const T* arr )
	{
		Set( arr );
		return *this;
	}

	Vector3& operator=( T* arr )
	{
		Set( arr );
		return *this;
	}

	inline void Set( const Vector3& other )
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	inline void Set( T X, T Y, T Z )
	{
		x = X;
		y = Y;
		z = Z;
	}

	inline void Set( T* arr )
	{
		x = arr[0];
		y = arr[1];
		z = arr[2];
	}

	//////////////////////////////////////////


	//////////////////////////////////////////
	// Casting
	//////////////////////////////////////////
	template< class U >
	operator Vector3< U >() { return Vector3< U >(
		static_cast< U >( x ),
		static_cast< U >( y ),
		static_cast< U >( z ) ); }
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////
	const T* GetAsArray() const
	{
		return &x;
	}

	T* GetAsArray()
	{
		int* r = new int[3];
		for ( int i = 0; i < 3; ++i ) r[i] = (*this)[i];
		return r;
	}

	T& operator[]( int i )
	{
		return *( &x + i );
	}

	const T& operator[]( int i ) const
	{
		return *( &x + i );
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Addition
	//////////////////////////////////////////
	Vector3 operator+( const Vector3& other ) const
	{
		return Vector3( x + other.x, y + other.y, z + other.z );
	}
	Vector3& operator+=( const Vector3& other )
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	//////////////////////////////////////////
	

	//////////////////////////////////////////
	// Subtraction
	//////////////////////////////////////////
	Vector3 operator-( const Vector3& other ) const
	{
		return Vector3( x - other.x, y - other.y, z - other.z );
	}
	Vector3& operator-=( const Vector3& other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Negation
	//////////////////////////////////////////
	Vector3 operator-() const
	{
		return Vector3( -x, -y, -z );
	}
	Vector3& Negate()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Dot product
	//////////////////////////////////////////
	T operator*( const Vector3& other ) const
	{
		return x * other.x + y * other.y + z * other.z;
	}
	T Dot( const Vector3& other ) const
	{
		return *this * other;
	}
	static T Dot( const Vector3& A, const Vector3& B )
	{
		return A * B;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Scalar multiplication
	//////////////////////////////////////////
	Vector3 operator*( const T S ) const
	{
		return Vector3( S * x, S * y, S * z );
	}
	Vector3& operator*=( const T S )
	{
		x *= S;
		y *= S;
		z *= S;
		return *this;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Length of vector
	//////////////////////////////////////////
	T Length() const
	{
		return sqrt( LengthSqr() );
	}
	T LengthSqr() const
	{
		return x*x + y*y + z*z;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Normalize this vector
	//////////////////////////////////////////
	Vector3& Normalize()
	{
		T len = Length();
		x /= len;
		y /= len;
		z /= len;
		return *this;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Cross Product
	//////////////////////////////////////////
	static Vector3 Cross( const Vector3& v1, const Vector3& v2 )
	{
		return Vector3( v1.y * v2.z - v1.z * v2.y,
						v1.z * v2.x - v1.x * v2.z,
						v1.x * v2.y - v1.y * v2.x );
	}
	//////////////////////////////////////////
};

//////////////////////////////////////////
// Allow scalar to be used as LHS
template< class T, class U >
Vector3< T > operator*( U S, const Vector3< T >& vector )
{
	return vector * static_cast< T >( S );
}
//////////////////////////////////////////


//////////////////////////////////////////
// Vector 2
//////////////////////////////////////////
template< class T >
class Vector2
{
public:
	// Components
	T x, y;

	//////////////////////////////////////////
	// Constructors
	//////////////////////////////////////////

	// Initialize all components
	explicit Vector2( T init=0 )
		: x( init ), y( init )
	{}

	// Set components
	Vector2( T x, T y )
		: x( x ), y( y )
	{}

	// From array
	explicit Vector2( T* a )
		: x( a[0] ), y( a[1] )
	{}

	// Copy
	Vector2( const Vector2& other )
		: x( other.x ), y( other.y )
	{}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Assignment
	//////////////////////////////////////////
	Vector2& operator=( const Vector2& other )
	{
		Set( other );
		return *this;
	}

	Vector2& operator=( const T* arr )
	{
		Set( arr );
		return *this;
	}

	Vector2& operator=( T* arr )
	{
		Set( arr );
		return *this;
	}

	inline void Set( const Vector2& other )
	{
		x = other.x;
		y = other.y;
	}

	inline void Set( T X, T Y )
	{
		x = X;
		y = Y;
	}

	inline void Set( T* arr )
	{
		x = arr[0];
		y = arr[1];
	}

	//////////////////////////////////////////


	//////////////////////////////////////////
	// Casting
	//////////////////////////////////////////
	template< class U >
	operator Vector2< U >() { return Vector2< U >(
		static_cast< U >( x ),
		static_cast< U >( y ) ); }
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////
	const T* GetAsArray() const
	{
		return &x;
	}

	T* GetAsArray()
	{
		int* r = new int[2];
		for ( int i = 0; i < 2; ++i ) r[i] = (*this)[i];
		return r;
	}

	T& operator[]( int i )
	{
		return *( &x + i );
	}

	const T& operator[]( int i ) const
	{
		return *( &x + i );
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Addition
	//////////////////////////////////////////
	Vector2 operator+( const Vector2& other ) const
	{
		return Vector2( x + other.x, y + other.y );
	}
	Vector2& operator+=( const Vector2& other )
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	//////////////////////////////////////////
	

	//////////////////////////////////////////
	// Subtraction
	//////////////////////////////////////////
	Vector2 operator-( const Vector2& other ) const
	{
		return Vector2( x - other.x, y - other.y );
	}
	Vector2& operator-=( const Vector2& other )
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Negation
	//////////////////////////////////////////
	Vector2 operator-() const
	{
		return Vector2( -x, -y, -z );
	}
	Vector2& Negate()
	{
		x = -x;
		y = -y;
		return *this;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Dot product
	//////////////////////////////////////////
	T operator*( const Vector2& other ) const
	{
		return x * other.x + y * other.y;
	}
	T Dot( const Vector2& other ) const
	{
		return *this * other;
	}
	static T Dot( const Vector2& A, const Vector2& B )
	{
		return A * B;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Scalar multiplication
	//////////////////////////////////////////
	Vector2 operator*( const T S ) const
	{
		return Vector2( S * x, S * y, S * z );
	}
	Vector2& operator*=( const T S )
	{
		x *= S;
		y *= S;
		return *this;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Length of vector
	//////////////////////////////////////////
	T Length() const
	{
		return static_cast< T >( std::sqrtl( LengthSqr() ) );
	}
	inline T LengthSqr() const
	{
		return x*x + y*y;
	}
	//////////////////////////////////////////


	//////////////////////////////////////////
	// Normalize this vector
	//////////////////////////////////////////
	Vector2& Normalize()
	{
		T len = Length();
		x /= len;
		y /= len;
		return *this;
	}
	//////////////////////////////////////////
};

//////////////////////////////////////////
// Allow scalar to be used as LHS
template< class T, class U >
Vector2< T > operator*( U S, const Vector2< T >& vector )
{
	return vector * static_cast< T >( S );
}
//////////////////////////////////////////