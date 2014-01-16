/*
 * Author      : Matthew Johnson
 * Date        : 08/Nov/2012
 * Description :
 *   General includes and misc. stuff.
 */

#pragma once

#define USE_CONSOLE

#include "window.h"
#include "console.h"

#include "math_util.h"
typedef Vector3< float > vec3;

#include "strutil.h"

#include "ResourceManager.h"

#include "Color.h"

void Text( int x, int y, const std::string& text, Color color=Color::WHITE );

// Prevent copying
#define PREVENT_COPYING( CLASS_NAME ) \
	private: \
	CLASS_NAME( const CLASS_NAME& ); \
	CLASS_NAME& operator=( const CLASS_NAME& );
