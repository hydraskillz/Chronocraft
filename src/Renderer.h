#pragma once

#include "main.h"
#include "Singleton.h"

#include "Cube.h"

namespace mzj
{

	class Renderer
		: public Singleton< Renderer >
	{
	public:
		Renderer();
		virtual ~Renderer();

		void ApplyTransformation( const Mat4f& view );
		void RenderCubeFaces( unsigned int faces, const Vec2f* texCoords );
		void RenderWireCube( float size=1.0f );

	private:
		// Primitives
		Cube cube;

	};

}