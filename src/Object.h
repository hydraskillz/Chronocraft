/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   Base class for Objects in the game world.
 */
#pragma once

//#include "TextureFactory.h"
#include "Texture.h"
#include "main.h"

namespace mzj
{
	
	class Object
	{
	public:
		Object();
		virtual ~Object();

		virtual void Render( const Mat4f& view );

		// Build transformation matrix
		void Transform( const Vec3f& trans, const Vec3f& rot );
		const Mat4f GetTransform() const;

		void Rotate( const Vec3f& rot );
		void Translate( const Vec3f& pos );

		// Apply translation/rotation (absolute)
		void ApplyTransform();
		// Apply translation/rotation (relative)
		void ApplyTransform( const Mat4f& view );

		// Parent/Unparent
		void SetParent( Object* parent );
		void ClearParent();

		Vec3f GetPosition() { return position; }
		void SetPosition( const Vec3f pos ) { position = pos; }

	protected:
		Vec3f* verts;			// Vertices
		Vec3f* normals;			// Normals
		Vec2f* texCoords;		// Texture Coords

		Mat4f transform;		// Transformation Matrix

		Vec3f position;			// Position Vector
		Vec3f rotation;			// Rotation Vector

		Object* parent;			// Parent to inherit transformations from

		Texture* texture;		// The texture mapped to this object
	};

}