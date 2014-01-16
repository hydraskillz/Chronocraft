/*
 * Author      : Matthew Johnson
 * Date        : 10/Nov/2012
 * Description :
 *   Base class for Actors in the game world.
 *   Similar to Objects, but with physics and non-static behavior.
 */
#pragma once

#include "Object.h"

namespace mzj
{

	class Actor
		: public Object
	{
	public:
		Actor( float drag=1.0f );
		virtual ~Actor();

		virtual void UpdateDynamics( double dt );

		// force to apply, offset angle to use
		void ApplyImpulse( float force, float angle=0.0f );
		// apply force vector
		void ApplyImpulse( const Vec3f& force );

		const Vec3f& GetVelocity() const { return velocity; }
		const Vec3f& GetRotation() const { return rotation; }

	protected:

		// Physics
		enum
		{
			BODY_STATIC = 0,
			BODY_DYNAMIC
		};
		Vec3f velocity;
		float drag;
		int physicsType;

	private:
		void ApplyMovement( Vec3f& movement );
		bool IsValidPosition( float x, float y, float z );
	};

}