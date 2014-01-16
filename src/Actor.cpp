#include "Actor.h"
#include "World.h"

using namespace mzj;

Actor::Actor( float drag )
	: Object()
	, drag( drag )
	, physicsType( BODY_STATIC )
{}

Actor::~Actor()
{}

void Actor::UpdateDynamics( double dt )
{
	if ( physicsType == BODY_DYNAMIC )
	{
		velocity.y -= 1.0f;
	}

	Vec3f movement = velocity * (float)dt;

	ApplyMovement( movement );

	velocity -= velocity * drag * (float)dt;
}

void Actor::ApplyMovement( Vec3f& movement )
{
	// No collision checks on static bodys
	if ( physicsType == BODY_STATIC )
	{
		position += movement;
		return;
	}

	// Floating point epsillon
	static const float E = 0.01f;
	// Movement step
	static const float deltaPercision = 0.1f;
	// Max steps
	static const int MAX_ITR = 20;

	Vec3f deltaPos;

	// Setup movement deltas
	for ( int c = 0; c < 3; c++ )
	{
		if ( std::abs( movement[c] ) > E )
		{
			if ( movement[c] >= 0 ) deltaPos[c] =  deltaPercision;
			else				    deltaPos[c] = -deltaPercision;
		}
	}

	// Apply movement in steps
	for ( int i = 0; i < MAX_ITR; ++i )
	{
		for ( int c = 0; c < 3; c++ )
		{
			// Set direction we are trying to move in
			Vec3f dir; dir[c] = 1;

			// Try to move
			if ( IsValidPosition( 
				position.x + deltaPos.x * dir[0],
				position.y + deltaPos.y * dir[1],
				position.z + deltaPos.z * dir[2] ) )
			{
				// Ok to move
				position[c] += deltaPos[c];
			}
			else
			{
				// Something stopped us
				velocity[c] = 0.0f;
			}
		}

		// Consume movement step
		movement -= deltaPos;

		// Check if we got stopped
		for ( int c = 0; c < 3; c++ )
		{
			if ( deltaPos[c] > 0 && movement[c] <= 0 ) deltaPos[c] = 0;
			if ( deltaPos[c] < 0 && movement[c] >= 0 ) deltaPos[c] = 0;

			// No more movement to apply in this direction
			if ( movement[c] == 0 ) deltaPos[c] = 0;
		}

		// No more movement to apply
		if ( movement.x == 0 && movement.y == 0 && movement.z == 0 ) break;
		if ( deltaPos.x == 0 && deltaPos.y == 0 && deltaPos.z == 0 ) break;
	}
}

bool Actor::IsValidPosition( float x, float y, float z )
{
	World& world = World::GetInstance();

	float startX = x - 0.4f;
	float startY = y - 0.9f;
	float startZ = z - 0.4f;
	float endX = x + 0.4f;
	float endY = y + 0.9f;
	float endZ = z + 0.4f;

	for ( float ix = startX; ix <= endX; ix += 0.5f )
		for ( float iy = startY; iy <= endY; iy += 0.5f )
			for ( float iz = startZ; iz <= endZ; iz += 0.5f )
			{
				Block& block = world.GetBlock( (int)ix, (int)iy, (int)iz );

				if ( block.GetType() != AIR )
					return false;
			}
	return true;
}

void Actor::ApplyImpulse( float force, float angle )
{
	const float ay = degreesToRadians( rotation.y + angle );
	velocity.Set( force * std::sinf( ay ), velocity.y, -force * std::cosf( ay ) );
}

void Actor::ApplyImpulse( const Vec3f& force )
{
	velocity = force;
}
