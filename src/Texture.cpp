#include "Texture.h"

//#include <Windows.h>
//#include <gl\GL.h>

#include <glut.h>

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

using namespace mzj;

Texture::Texture()
	: m_uTextureId( 0 )
	, m_bLoaded( false )
{}


Texture::Texture( const std::string& texturePath )
	: m_uTextureId( 0 )
	, m_bLoaded( false )
{
	LoadFromFile( texturePath );
}


Texture::~Texture()
{
	AssertTextureIdFree();
}


Texture::Size Texture::GetWidth() const
{
	return m_Width;
}


Texture::Size Texture::GetHeight() const
{
	return m_Height;
}


bool Texture::HasAlpha() const
{
	return m_bAlpha;
}


void Texture::LoadFromMemory( Size width, Size height, Color* pRGBATexels )
{
	m_bAlpha = true;

	m_sTexturePath.clear();
	CreateOpenGLTexture( width, height, pRGBATexels );

	m_bLoaded = true;
}


///////////////////////////////////////////////////////////////////////////////
// Load texture from filepath
///////////////////////////////////////////////////////////////////////////////
void Texture::LoadFromFile( const std::string& texturePath )
{
	int width, height, nChannels;
	unsigned char* const texels = stbi_load( texturePath.c_str(), &width, &height, &nChannels, 4 );

	//assert( texels );

	if ( texels )
	{
		m_bAlpha = nChannels == 4;

		m_sTexturePath = texturePath;

		CreateOpenGLTexture( width, height, texels );

		stbi_image_free( texels );

		m_bLoaded = true;
	}
}


///////////////////////////////////////////////////////////////////////////////
// Creates and configures an OpenGL texture from the texels
///////////////////////////////////////////////////////////////////////////////
void Texture::CreateOpenGLTexture( Size width, Size height, void* texels )
{
	AssertTextureIdFree();

	m_Width  = width;
	m_Height = height;

	// Create OpenGL texture object
	glGenTextures( 1, &m_uTextureId );

	// Configure the texture
	glBindTexture( GL_TEXTURE_2D, m_uTextureId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Load texture object from memory
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels );
}


///////////////////////////////////////////////////////////////////////////////
// Ensures that the OpenGl texture has been freed
///////////////////////////////////////////////////////////////////////////////
void Texture::AssertTextureIdFree()
{
	if ( m_uTextureId > 0 )
		glDeleteTextures( 1, &m_uTextureId );
	m_bLoaded = false;
}
