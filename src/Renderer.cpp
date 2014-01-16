#include "Renderer.h"
#include "gl.h"

#include <fstream>

using namespace mzj;

Renderer::Renderer()
	: INIT_SINGLETON( Renderer )
{
}

Renderer::~Renderer()
{}

void Renderer::ApplyTransformation( const Mat4f& view )
{
	glLoadMatrixf( view.GetArray() );
}

void Renderer::RenderCubeFaces( unsigned int faces, const Vec2f* texCoords )
{
	glColor3f( 1, 1, 1 );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	if ( texCoords )
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, 0, cube.GetVertexPointer() );

	glNormalPointer( GL_FLOAT, 0, cube.GetNormalPointer() );

	if ( texCoords )
	{
		glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
	}
	glDrawArrays( GL_TRIANGLES, 0 , 36 );
	/*
	if ( faces & Front ) glDrawArrays( GL_TRIANGLES, 0, 6 );
	if ( faces & Right ) glDrawArrays( GL_TRIANGLES, 6, 6 );
	if ( faces & Top   ) glDrawArrays( GL_TRIANGLES, 12, 6 );
	if ( faces & Left  ) glDrawArrays( GL_TRIANGLES, 18, 6 );
	if ( faces & Bottom) glDrawArrays( GL_TRIANGLES, 24, 6 );
	if ( faces & Back  ) glDrawArrays( GL_TRIANGLES, 30, 6 );
	*/
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	if ( texCoords )
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void Renderer::RenderWireCube( float size )
{
	glColor3f( 1.0f, 0.0f, 0.0f );
	glutWireCube( size );
}