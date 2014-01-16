/*
 * Author      : Matthew Johnson
 * Date        : 31/Aug/2012
 * Description :
 *   Loads Textures.
 */
#pragma once

#include <string>

namespace mzj
{

	class Texture
	{
	public:
		typedef unsigned int Size;
		typedef unsigned int Color;

		Texture();
		explicit Texture( const std::string& texturePath );
		~Texture();

		bool IsLoaded() const { return m_bLoaded; }
		std::string GetFilePath() const { return ( IsLoaded() && m_sTexturePath.size() ) ? m_sTexturePath : std::string(); }
		Size GetWidth() const;
		Size GetHeight() const;
		unsigned int GetTextureId() const { return m_uTextureId; }
		
		bool HasAlpha() const;
		
		void LoadFromMemory( Size width, Size height, Color* pRGBATexels );
		
		void LoadFromFile( const std::string& texturePath );

	private:

		Size m_Width;
		Size m_Height;
		std::string m_sTexturePath;
		unsigned int m_uTextureId;
		bool m_bAlpha;
		bool m_bLoaded;

		void CreateOpenGLTexture( Size width, Size height, void* texels );
		inline void AssertTextureIdFree();

		// Prevent copying
		Texture( const Texture& );
		Texture& operator=( const Texture& );
	};

}