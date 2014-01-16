/*
 * Author      : Matthew Johnson
 * Date        : 13/Nov/2012
 * Description :
 *   Container for chunks.
 */
#pragma once

#include "Chunk.h"
#include "ThreadSafeQueue.h"

namespace mzj
{

	class Terrain
	{
	public:
		Terrain();
		~Terrain();

		int Render( const Camera& view );

		void GenChunkBlocks( Vec2i xRange, Vec2i zRange );
		void GenChunkLighting( const Vec2i& chunkPos );
		
		void PostChunkVBOUpdate( int x, int z );
		void PostChunkVBOUpdate( Chunk* chunk );

		// Generates chunk VBOs
		void RenderThread();

		// Vector forwards
		Chunk& GetChunk( const Vec2i& pos ) { return GetChunk( pos.x, pos.y ); }
		const Chunk& GetChunk( const Vec2i& pos ) const { return GetChunk( pos.x, pos.y ); }

		// Chunk accessors
		Chunk& GetChunk( int x, int z );
		const Chunk& GetChunk( int x, int z ) const;
		Block& GetBlock( int x, int y, int z );
		const Block& GetBlock( int x, int y, int z ) const;

		static const int MAX_X_CHUNKS = 20;
		static const int MAX_Z_CHUNKS = 20;

	private:
		Chunk chunks[MAX_X_CHUNKS][MAX_Z_CHUNKS];

		// VBO updates posted by generation thread
		ThreadSafeQueue< Chunk* > chunkVBOQueue;
		// VBO updates posted from main thread
		ThreadSafeQueue< Chunk* > chunkVBOQueueUser;
	};

}