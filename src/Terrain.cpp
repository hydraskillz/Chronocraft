#include "Terrain.h"

using namespace mzj;

Terrain::Terrain()
{}

Terrain::~Terrain()
{}

int Terrain::Render( const Camera& view )
{
	int renderedTris = 0;
	for ( int x = 0; x < MAX_X_CHUNKS; ++x )
		for ( int z = 0; z < MAX_Z_CHUNKS; ++z )
		{
			renderedTris += chunks[x][z].Render( view, x, z );
		}
	return renderedTris;
}

void Terrain::RenderThread()
{
	Chunk* chunk = nullptr;
	while ( true )
	{
		// Deal with user posted updates first
		while ( chunkVBOQueueUser.Pop( chunk ) )
		{
			chunk->GenerateRenderData();
		}

		// Generation updates
		if ( chunkVBOQueue.Pop( chunk ) )
			chunk->GenerateRenderData();
		else
		{
			// TODO Sleep
		}
	}
}

void Terrain::GenChunkBlocks( Vec2i xRange, Vec2i zRange )
{
	// Limit values...
	if ( xRange.y > MAX_X_CHUNKS ) xRange.y = MAX_X_CHUNKS;
	if ( zRange.y > MAX_Z_CHUNKS ) zRange.y = MAX_X_CHUNKS;
	if ( xRange.x < 0 ) xRange.x = 0;
	if ( zRange.x < 0 ) zRange.x = 0;

	// Shape pass
	for ( int x = xRange.x; x < xRange.y; ++x )
	{
		for ( int z = zRange.x; z < zRange.y; ++z )
		{
			chunks[x][z].Generate( x, z );
		}
	}

	// Detail pass
	for ( int x = xRange.x; x < xRange.y; ++x )
	{
		for ( int z = zRange.x; z < zRange.y; ++z )
		{
			chunks[x][z].GenFeatures( x, z );
		}
	}
		
	// Light pass
	for ( int x = xRange.x; x < xRange.y; ++x )
	{
		for ( int z = zRange.x; z < zRange.y; ++z )
		{
			chunks[x][z].GenLighting();
		}
	}

	// Post Updates
	for ( int x = xRange.x; x < xRange.y; ++x )
	{
		for ( int z = zRange.x; z < zRange.y; ++z )
		{
			chunkVBOQueue.Push( &chunks[x][z] );
		}
	}
}

void Terrain::GenChunkLighting( const Vec2i& chunkPos )
{
	const Vec2i dirs[9] =
	{
		Vec2i( 0, 0 ),
		Vec2i( 0, 1 ),
		Vec2i( 1, 0 ),
		Vec2i( 0, -1 ),
		Vec2i( -1, 0 ),
		Vec2i( 1, 1 ),
		Vec2i( -1, -1 ),
		Vec2i( -1, 1 ),
		Vec2i( 1, -1 ),
	};

	for ( int i = 0; i < 9; ++i )
	{
		Vec2i neighbor = chunkPos + dirs[i];
		GetChunk( neighbor ).GenLighting();
	}
	for ( int i = 0; i < 9; ++i )
	{
		Vec2i neighbor = chunkPos + dirs[i];
		GetChunk( neighbor ).GenLightingSpread();
	}
	for ( int i = 0; i < 9; ++i )
	{
		Vec2i neighbor = chunkPos + dirs[i];
		// This is a user thread function so use the user queue
		PostChunkVBOUpdate( &GetChunk( neighbor ) );
	}
}

void Terrain::PostChunkVBOUpdate( int x, int z )
{
	chunkVBOQueueUser.Push( &chunks[x][z] );
}

void Terrain::PostChunkVBOUpdate( Chunk* chunk )
{
	chunkVBOQueueUser.Push( chunk );
}

Chunk& Terrain::GetChunk( int x, int z )
{
	if ( x < 0 || x >= MAX_X_CHUNKS || z < 0 || z >= MAX_Z_CHUNKS )
		return Chunk::NULL_CHUNK;
	return chunks[x][z];
}

const Chunk& Terrain::GetChunk( int x, int z ) const
{
	if ( x < 0 || x >= MAX_X_CHUNKS || z < 0 || z >= MAX_Z_CHUNKS )
		return Chunk::NULL_CHUNK;
	return chunks[x][z];
}

Block& Terrain::GetBlock( int x, int y, int z )
{
	Chunk& chunk = GetChunk( x / Chunk::X_SIZE, z / Chunk::Z_SIZE );
	return chunk.GetBlock( x % Chunk::X_SIZE, y, z % 16 );
}

const Block& Terrain::GetBlock( int x, int y, int z ) const
{
	const Chunk& chunk = GetChunk( x / Chunk::X_SIZE, z / Chunk::Z_SIZE );
	return chunk.GetBlock( x % Chunk::X_SIZE, y, z % 16 );
}
