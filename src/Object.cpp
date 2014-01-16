#include "Object.h"
#include "TextureFactory.h"
#include <glut.h>

using namespace mzj;

Object::Object()
	: verts( nullptr )
	, normals( nullptr )
	, texCoords( nullptr )
	, position()
	, rotation()
	, parent( nullptr )
	, texture( nullptr )
{}

Object::~Object()
{
	if ( verts ) delete verts;
	if ( normals ) delete normals;
	if ( texCoords ) delete texCoords;
	parent = nullptr;
	texture = nullptr;
}

void Object::Render( const Mat4f& view )
{
	glColor3f( 1, 1, 1 );	// TODO colors??

	if ( verts )
		glEnableClientState( GL_VERTEX_ARRAY );
	if ( normals )
		glEnableClientState( GL_NORMAL_ARRAY );
	if ( texCoords )
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	if ( verts )
		glVertexPointer( 3, GL_FLOAT, 0, verts );
	if ( normals )
		glNormalPointer( GL_FLOAT, 0, normals );
	if ( texCoords )
		glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );

	ApplyTransform( view );

	TextureFactory::ApplyTexture( texture );
	
	glDrawArrays( GL_TRIANGLES, 0, 36 );

	if ( verts )
		glEnableClientState( GL_VERTEX_ARRAY );
	if ( normals )
		glEnableClientState( GL_NORMAL_ARRAY );
	if ( texCoords )
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		
}

void Object::Transform( const Vec3f& trans, const Vec3f& rot )
{
	Mat4f T, Rx, Ry;
	T.Translate( -trans );
	Ry.Rotate( rot.y, Vec3f( 0, 1, 0 ) );
	Rx.Rotate( rot.x, Vec3f( 1, 0, 0 ) );

	transform = T * Ry * Rx;
}

const Mat4f Object::GetTransform() const
{
	if ( parent )
		return transform * parent->transform;
	return transform;
}

void Object::Rotate( const Vec3f& rot )
{
	rotation += rot;
	rotation.x = clamp( rotation.x, -90.0f, 90.0f );
}

void Object::Translate( const Vec3f& pos )
{
	position = -pos;
}

void Object::ApplyTransform()
{
	Transform( position, rotation );
	if ( parent )
		glLoadMatrixf( ( transform * parent->GetTransform() ).GetArray() );
	else
		glLoadMatrixf( transform.GetArray() );
}

void Object::ApplyTransform( const Mat4f& view )
{
	Transform( position, rotation );
	if ( parent )
	{
		parent->ApplyTransform();
		glLoadMatrixf( ( transform * parent->GetTransform() * view ).GetArray() );
	}
	else
		glLoadMatrixf( ( transform * view ).GetArray() );
}

void Object::SetParent( Object* parent )
{
	this->parent = parent;
}

void Object::ClearParent()
{
	parent = nullptr;
}
