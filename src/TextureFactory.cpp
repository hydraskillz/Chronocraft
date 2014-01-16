#include "TextureFactory.h"
#include "Texture.h"
#include "main.h"

#include <map>
#include <glut.h>

using namespace mzj;

namespace
{
	static std::map< std::string, Texture* > TextureMap;
	typedef std::pair< std::string, Texture* > TexMapPair;
}

void TextureFactory::ApplyTexture( const Texture* pTexture )
{
	if ( pTexture == nullptr )
	{
		glDisable( GL_TEXTURE_2D );
	}
	else
	{
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, pTexture->GetTextureId() );

		// Texture has alpha, so enable alpha blending
		if ( pTexture->HasAlpha() )
		{
			glEnable( GL_BLEND );
//			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		// No alpha, disable alpha blending
		else
			glDisable( GL_BLEND );
	}
}

Texture* TextureFactory::CreateOrGetTexture( const std::string& texturePath )
{
	Texture* result = nullptr;

	auto iter = TextureMap.find( texturePath );

	// Found texture in TextureMap
	if ( iter != TextureMap.end() )
		result = iter->second;
	// Texture not in TextureMap, create new texture and register it
	else
	{
		result = new Texture( texturePath );
		TextureMap.insert( TexMapPair( texturePath, result ) );
	}

	return result;
}

void TextureFactory::DestroyTexture( Texture*& pTexture )
{
	// Nothing to delete...
	if ( pTexture == nullptr ) return ;

	auto iter = TextureMap.find( pTexture->GetFilePath() );

	// Texture was be registered in the TextureMap
	if ( iter != TextureMap.end() )
	{
		// Delete texture and remove entry from TextureMap
		delete iter->second;
		TextureMap.erase( iter );
	}
	// Texture was NOT registered in the TextureMap
	else
		delete pTexture;
	
	pTexture = nullptr;
}

void TextureFactory::DestroyAllTextures()
{
	auto iter = TextureMap.begin();
	auto end  = TextureMap.end();
	while ( iter != end )
	{
		delete iter->second;
		iter = TextureMap.erase( iter );
	}
}
