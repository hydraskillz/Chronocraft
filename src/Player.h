/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   The player. Has a camera used as the viewport.
 */
#pragma once

#include "Actor.h"
#include "Camera.h"
#include "InputListener.h"
#include "Item.h"

namespace mzj
{

	class Player
		: public Actor, public InputListener
	{
	public:
		Player();
		virtual ~Player();

		virtual void OnButtonDown    ( Button code );
		virtual void OnButtonJustUp  ( Button code );
		virtual void OnButtonJustDown( Button code );
		virtual void OnAxisChanged   ( Axis which, float x, float y );
		virtual void OnMouseMove     ( int x, int y );

		virtual void UpdateDynamics( double dt );
		//virtual void Render( const Mat4f& view );

		Camera& GetCamera() { return camera; }

		Vec3f playerGetFacingVector()
		{
			const float t = degreesToRadians( rotation.y );
			const float p = degreesToRadians( rotation.x + 90 );
			return Vec3f( std::sinf( p ) * std::sinf( t ),
						  std::cosf( p ),
						 -std::sinf( p ) * std::cosf( t ));
		}

		const std::string& GetActiveItemName() const { return activeItem->GetName(); }
		int GetItemType() const { return activeItem->GetType(); }
		bool OnUse( Block* block, const Vec3f& position, const Vec3f& norm )
		{
			return activeItem->OnUse( block, position, norm );
		}

		float GetBobOffset() const { return cameraBob; }

	private:
		Camera camera;
		float speed;
		bool move[4];
		bool jump;
		float jumpDelay;

		float cameraBobAngle;
		float cameraBob;
		
		bool sprint;

		Item* activeItem;

		std::string toolbar[9];
	};

}