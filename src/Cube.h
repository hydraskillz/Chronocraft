/*
 * Author      : Matthew Johnson
 * Date        : 14/Nov/2012
 * Description :
 *   A cube.
 */
#pragma once

#include "math_util.h"

namespace mzj
{
	enum Face
	{
		Front  = 0x0001,
		Right  = 0x0002,
		Top    = 0x0004,
		Left   = 0x0008,
		Bottom = 0x0010,
		Back   = 0x0020,
		All    = 0x003F
	};

	class Cube
	{
	public:
		Cube( float size=1.0f );
		~Cube();

//		Vec3f verts[36];
		const float size;

		const Vec3f& GetCubeVertex ( unsigned int i ) { return verts[ i ]; }
		const Vec2f& GetCubeTexVert( unsigned int i ) { return texCoords[ i ]; }
		const Vec3f& GetCubeNormal ( unsigned int i ) { return normals[ i ]; }

		const Vec3f* GetVertexPointer() const { return verts; }
		const Vec3f* GetNormalPointer() const { return normals; }

	private:
		void BuildVerts();

		Vec3f verts[36];
		Vec2f texCoords[36];
		Vec3f normals[36];
	};

}