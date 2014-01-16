#include "World.h"
#include "InputController.h"
#include "Renderer.h"
#include "GLThread.h"
#include "Thread.h"

#include "Item.h"

#include <algorithm>

using namespace mzj;

World::World()
	: INIT_SINGLETON( World )
	, use( false )
{
	//terrain.GenChunkBlocks();
	//terrain.GenChunkLighting();

	chunkLoadRange[0].Set( 0, 5 );
	chunkLoadRange[1].Set( 0, 5 );

	// Thread for generating chunk data
	// Posts chunk VBO updates to render queue
	chunkBuilderThread = new Thread( [&]() {
		terrain.GenChunkBlocks( chunkLoadRange[0], chunkLoadRange[1] );
	});
	/*
	// Generates lighing data for chunks
	chunkLightingThread = new Thread( [&]() {
		terrain.GenChunkLighting();
	});
	*/
	// Thread to render chunks VBOs via a queue
	chunkGLRenderUpdates = new GLThread( [&]() {
		terrain.RenderThread();
	});

	/*
	// Thread to generate all chunks
	chunkGLThread = new GLThread( [&]() {
		terrain.GenRenderData();
	});
	*/

	/*
	// Thread to render all chunks
	chunkGLDetail = new GLThread( [&]() {
		terrain.GenRenderDetail();
	});
	*/
	
	InputController::GetInstance().RegisterListener( this );
}

World::~World()
{
	delete chunkBuilderThread;
//	delete chunkLightingThread;
//	delete chunkGLThread;
	delete chunkGLRenderUpdates;
	
	InputController::GetInstance().UnRegisterListener( this );
}

void World::UpdateDynamics( double dt )
{
	player.UpdateDynamics( dt );

	// Player/Camera transformation
	player.GetCamera().ApplyTransform();

	//
	// Dynamic chunk loading relative to player position
	//

	Vec3i pos = player.GetPosition();
	pos.x /= Chunk::X_SIZE;
	pos.y /= Chunk::Y_SIZE;
	pos.z /= Chunk::Z_SIZE;

	const Chunk& chunkX = terrain.GetChunk( pos.x + 8, pos.z );
	const Chunk& chunkZ = terrain.GetChunk( pos.x, pos.z + 8 );

	// See if a chunk ahead of us is not generated yet
	if ( chunkX.IsGenerated() == false || chunkZ.IsGenerated() == false )
	{
		chunkLoadRange[0].Set( pos.x - 10, pos.x + 10 );
		chunkLoadRange[1].Set( pos.z - 10, pos.z + 10 );
		
		// Build chunks that just came into visible range
		if ( chunkBuilderThread->Finished() )
		{
			delete chunkBuilderThread;
			chunkBuilderThread = new Thread( [&]() {
				terrain.GenChunkBlocks( chunkLoadRange[0], chunkLoadRange[1] );
			});
		}
	}

	Block::NULL_BLOCK.SetType( AIR );
}

void World::Render()
{
	static int tris = 0;

	Vec3f playerPos = player.GetPosition();
	Vec3f playerFacing = player.playerGetFacingVector();
	Block& block = GetBlock( (Vec3i)playerPos );

	playerPos.y += 0.5f;	// Offset player height TODO make a constant or something
	RayCastResult rc = RayCastTerrain( playerPos, playerFacing );
	
	if ( *rc.block != Block::NULL_BLOCK )
	{
		// Draw selection
		Mat4f trans;
		trans.Translate( Vec3f( (int)(rc.hitpos.x)+0.5f, (int)rc.hitpos.y + 0.5f, (int)(rc.hitpos.z)+0.5f ) );
		Renderer::GetInstance().ApplyTransformation( trans * player.GetCamera().GetTransform() );
		Renderer::GetInstance().RenderWireCube(1.001f); // Avoid z fighting

		// Ray hit position
		trans.Translate( rc.hitpos );
		Renderer::GetInstance().ApplyTransformation( trans * player.GetCamera().GetTransform() );
		Renderer::GetInstance().RenderWireCube( 0.1f );
		
		if ( use && player.OnUse( rc.block, rc.hitpos, rc.normal ) )
		{
			// Blocks where changed... update all chunks touching the block
			Vec2i chunkPos( 
				static_cast< int >( playerPos.x / Chunk::X_SIZE ),
				static_cast< int >( playerPos.z / Chunk::Z_SIZE ) );

			terrain.GenChunkLighting( chunkPos );

			/*
			for ( int x = -1; x <= 1; ++x )
			{
				for ( int z = -1; z <= 1; ++z )
				{
					// Only need to check these chunks
					// 0 1 0
					// 1 1 1
					// 0 1 0
					if ( x == 0 || z == 0 )
					{
						Vec2i chunkPos( (int)( (rc.hitpos.x+x) / Chunk::X_SIZE), (int)( (rc.hitpos.z+z) / Chunk::Z_SIZE) );
						Chunk& chunk = terrain.GetChunk( chunkPos );
						
						if ( chunk.needsRebuild == false )
						{
							dirtyChunks.push_back( &chunk );
							Print( "Chunk: " << chunkPos.x << ", " << chunkPos.y << " marked for rebuild\n" );
						}
						chunk.needsRebuild = true;
						
						//terrain.updateLights = true;
						//terrain.updateVBOs = true;
					}
				}
			}*/

			/*
			for ( auto itr = dirtyChunks.begin(); itr != dirtyChunks.end(); ++itr )
			{
				//terrain.PostChunkVBOUpdate( *itr );
				terrain.PostChunkLightUpdate( *itr );
			}
			*/
		}
	}

	tris = terrain.Render( player.GetCamera() );

	// Text overlays
	std::string type = block.GetTypeString(); 
	Text( 10, WIN_HEIGHT - 20,
		std::string( "P: " ) + ToString( playerPos ) + 
		" C: " + IntToString( (int)( playerPos.x / Chunk::X_SIZE ) ) + 
		", " + IntToString( (int)( playerPos.z / Chunk::Z_SIZE ) ) + 
		" B: " + type );
	Text( 10, WIN_HEIGHT - 40, std::string( "Tri: " ) + IntToString( tris ) );

	Text( 10, WIN_HEIGHT - 60, std::string( "RayTrace: " ) + (*rc.block!=Block::NULL_BLOCK ? ( rc.block->GetTypeString() + " Hit: " + ToString( rc.hitpos ) + " N: " + ToString( rc.normal ) ) : "None") );

	Text( 10, WIN_HEIGHT - 80, std::string( "Facing: " ) + ToString( playerFacing ) );

	Text( 10, WIN_HEIGHT - 100, std::string( "Active Item: " ) + player.GetActiveItemName() );

	use = false;
}

void World::OnButtonDown( Button code )
{
}

void World::OnButtonJustUp  ( Button code )
{
}

void World::OnButtonJustDown( Button code )
{
	switch ( code )
	{
	case KEY_LMOUSE:
		//killBlock = true;
		use = true;
		break;
	case KEY_RMOUSE:
		break;
	}
}

void World::OnAxisChanged( Axis which, float x, float y )
{
}

void World::OnMouseMove( int x, int y )
{
}

const Mat4f World::GetTransformMatrix()
{
	return player.GetCamera().GetTransform();
}

Block& World::GetBlock( int x, int y, int z )
{
	return terrain.GetBlock( x, y, z );
}

const Block& World::GetBlock( int x, int y, int z ) const
{
	return terrain.GetBlock( x, y, z );
}

Block& World::GetBlock( const Vec3i& position )
{
	return GetBlock( position.x, position.y, position.z );
}

const Block& World::GetBlock( const Vec3i& position ) const
{
	return GetBlock( position.x, position.y, position.z );
}

Chunk& World::GetChunk( int x, int z )
{
	return terrain.GetChunk( x, z );
}

const Chunk& World::GetChunk( int x, int z ) const
{
	return terrain.GetChunk( x, z );
}

// Gets the first non air block the ray intersects
RayCastResult World::RayCastTerrain( const Vec3f& origin, const Vec3f& dir, float length )
{
	RayCastResult result;

	Vec3f position = origin;
	Vec3f direction = dir; direction.Normalize();
	const float E = 0.001f;
	const float step = 0.1f;	// 1 tenth of a block

	while ( ( position - origin ).Length() < length )
	{
		/* Simple stepping method. Can clip corners of blocks (rarely). Close enough. */

		// Next position
		Vec3f positionNext = position + direction * step;

		// Hit something?
		result.block = &GetBlock( positionNext );

		// Is it a block?
		if ( result.block->GetType() != AIR )
		{
			// Return position of hit and normal
			result.hitpos = positionNext;
			result.normal = (Vec3i)position - (Vec3i)positionNext;

			break;
		}

		// Next step
		result.block = &Block::NULL_BLOCK;
		position = positionNext;

		/* A different approach using point plane intersection. There is a bug here and it doesn't work 100% correctly */
		/*
		// Check each of the 3 planes we could intersect
		float deltas[3];
		Vec3f normals[3];
		for ( int axis = 0; axis < 3; ++axis )
		{
			Vec3f normal;
			normal[ axis ] = direction[ axis ] < 0.0f ? 1.0f : -1.0f;

			Vec3f plane(
				std::floor( position.x ),
				std::floor( position.y ),
				std::floor( position.z ) );

			plane[ axis ] += direction[ axis ] < 0.0f ? -1.0f : 1.0f;
			plane[ ( axis + 1) % 3 ] += direction[ ( axis + 1) % 3 ] < 0.0f ? -0.5f : 0.5f;
			plane[ ( axis + 2) % 3 ] += direction[ ( axis + 2) % 3 ] < 0.0f ? -0.5f : 0.5f;

			//plane[ axis ] -= normal[ axis ];
			

			float A = ( plane - position ) * normal;
			float B = direction * normal;
			float d;

			// Get distance to plane

			// Miss
			if ( std::abs( B ) < E && std::abs( A ) > E )
			{
				d = 1;
			}
			// On
			else if ( std::abs( B ) < E && std::abs( A ) < E )
			{
				d = 1;
			}
			// Hit
			else
			{
				d = A / B;
			}

			deltas[ axis ] = d;
			normals[ axis ] = normal;
		}
		
		// Get shortest distance
		float minDelta = 9999;
		int minAxis = 0;
		for ( int i = 0; i < 3; ++i )
		{
			if ( deltas[ i ] < minDelta )
			{
				minDelta = deltas[ i ];
				minAxis = i;
			}
		}
		
		Vec3f positionNext = position + direction * minDelta;

		result.block = &GetBlock( positionNext );

		if ( result.block->GetType() != AIR )
		{
			result.hitpos = positionNext;
			result.normal = normals[ minAxis ];
			//result.normal[ minAxis ] = direction[ minAxis ] < 0 ? 1.0f : -1.0f;
			
			break;
		}

		result.block = &Block::NULL_BLOCK;
		position = positionNext;
		*/
	}

	return result;
}
