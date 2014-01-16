#pragma once

#include <sstream>
#include <string>
#include <vector>

template< class T >
inline std::string ToString( const Vector3< T >& vec )
{
	std::stringstream ss;
	ss << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
	return ss.str();
}

inline std::string IntToString( int v )
{
	std::stringstream ss;
	ss << v;
	return ss.str();
}

template< class T >
inline bool StringToType( const std::string& str, T* type )
{
	std::stringstream ss;
	ss << str;
	ss >> *type;
	return true;
}