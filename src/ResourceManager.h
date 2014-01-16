/*
 * Author      : Matthew Johnson
 * Date        : 11/Nov/2012
 * Description :
 *   Manages access of resource for cross-platform support.
 */
#pragma once

#include <string>

namespace mzj
{
	enum RESOURCE_TYPE
	{
		RES_DATA    = 0,	// Root folder
		RES_TEXTURE = 1,	// Texture folder
		RES_OBJECT  = 2,	// Object folder
		RES_AUDIO   = 3,	// Audio foler
		RES_TEXT    = 4,	// Text folder
	};

	namespace ResourceManager
	{
		std::string GetResourcePath( unsigned int RES_ID=RES_DATA );
	}
}