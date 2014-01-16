/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   The world controls updating and draw calls to game objects.
 */
#pragma once

#include "Singleton.h"
#include "InputListener.h"

#include "Player.h"
#include "Terrain.h"

#include <vector>

namespace mzj
{
	class Terrain;
	class GLThread;
	class Thread;
	
	// Returned info from raycast
	struct RayCastResult
	{
		Vec3f hitpos;
		Vec3f normal;
		Block* block;
	};

	class World
		: public InputListener, public Singleton< World >
	{
	public:
		World();
		virtual ~World();

		// Update physics
		void UpdateDynamics( double dt );
		// Draw calls
		void Render();

		// Input
		virtual void OnButtonDown    ( Button code );
		virtual void OnButtonJustUp  ( Button code );
		virtual void OnButtonJustDown( Button code );
		virtual void OnAxisChanged( Axis which, float x, float y );
		virtual void OnMouseMove( int dx, int dy ) ;

		// View transformation
		const Mat4f GetTransformMatrix();

		// Player accessors
		const Vec3f GetPlayerPos() { return player.GetPosition(); }
		Player GetPlayer() { return player; }

		// Block accessros
		Block& GetBlock( int x, int y, int z );
		const Block& GetBlock( int x, int y, int z ) const;
		Block& GetBlock( const Vec3i& position );
		const Block& GetBlock( const Vec3i& position ) const;
		// Chunk accessors
		Chunk& GetChunk( int x, int z );
		const Chunk& GetChunk( int x, int z ) const;

		// Raycast into the block grid
		RayCastResult RayCastTerrain( const Vec3f& origin, const Vec3f& direction, float length=8.0f );

		unsigned int GetCurrentLightLevel() const { return lightLevel; }

	private:
		Player player;

		Terrain terrain;

		bool use;	// TODO move into player input section

		
		Thread* chunkBuilderThread;
		//Thread* chunkLightingThread;

		//GLThread* chunkGLThread;
		//GLThread* chunkGLDetail;
		GLThread* chunkGLRenderUpdates;

		Vec2i chunkLoadRange[2];

		unsigned int lightLevel;
	};

}