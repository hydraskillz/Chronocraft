/*
 * Author      : Matthew Johnson
 * Date        : 11/Nov/2012
 * Description :
 *   Loads and manages Textures.
 */
#pragma once

#include "Texture.h"

#include <string>

namespace mzj
{
	
	namespace TextureFactory
	{
		void ApplyTexture( const Texture* pTexture );
		Texture* CreateOrGetTexture( const std::string& texturePath );
		void DestroyTexture( Texture*& pTexture );
		void DestroyAllTextures();
	}
	
}