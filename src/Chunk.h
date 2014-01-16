/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   A chunk is a collection of blocks.
 */
#pragma once

#include "Block.h"
#include "Camera.h"
#include <cassert>

namespace mzj
{
	class Texture;

	class Chunk
	{
	public:
		Chunk();
		~Chunk();

		bool Generate( int cx, int cz );
		bool GenFeatures( int cx, int cz );

		bool GenLighting();
		bool GenLightingSpread();

		int Render( const Camera& view, int x, int z );

		const Block& GetBlock( int x, int y, int z ) const;
		
		Block& GetBlock( int x, int y, int z );

		bool SetBlockType( float x, float y, float z, BlockTypeId id );

		bool IsGenerated() const { return generationDone; }

		inline Vec3i& Chunk::LocalToWorld( Vec3i& local ) const;

		template< typename operation_t >
		void ForEachBlock( operation_t op )
		{
			for ( int x = 0; x < X_SIZE; ++ x )
			for ( int y = 0; y < Y_SIZE; ++ y )
			for ( int z = 0; z < Z_SIZE; ++ z )
			{
				op( GetBlock( x, y, z ), x, y, z );
			}
		}

		bool GenerateRenderData();

		static const int X_SIZE = 16;
		static const int Y_SIZE = 128;
		static const int Z_SIZE = X_SIZE;

		static const int LIGHT_SKY = 15;
		static const int LIGHT_MAX = LIGHT_SKY - 1;

		static Chunk NULL_CHUNK;

		bool needsRebuild;
		bool genDetailDone;
		
	private:
		BlockTypeId GenerateBlockType( int x, int y, int z );

		void GenBedrock();
		void GenTree( int x, int y, int z);
		
		void EmitCubeFace(
				std::vector< float >& inOutVertexElem,
				const Vec3f& faceNormal,
				const Vec3f& faceTranslation,
				const Vec2f& faceTextureTranslation,
				float faceTextureScale );

		void Chunk::EmitCubeFaces(
			std::vector< float >& inOutVertexElem,
			unsigned int faces,
			BlockTypeId id,
			const Vec3f& translation,
			const Vec3f color[6] );

		void DestroyRenderData();

		template< class T, class TAlpha >
		T lerp( T min, T max, TAlpha alpha )
		{
			return static_cast< T > ( min + ( max - min ) * alpha );
		}

		void GetNeighbors( const Vec3i& origin, std::vector< const Block* >& neighbors );

		unsigned int idVBO;
		unsigned int numVerts;
		Texture* texture;
		static const float TEXTURE_DELTA;

		int cx;
		int cz;

		bool generationDone;

		bool lock;
		bool renderLock;
		bool lightLock;

		Block blocks[X_SIZE][Y_SIZE][Z_SIZE];
	};

}