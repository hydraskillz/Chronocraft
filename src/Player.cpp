#include "Player.h"
#include "InputController.h"
#include "World.h"

#include "ItemDB.h"

using namespace mzj;

Player::Player()
	: Actor( 9.5f )
	, speed( 5.0f )
	, cameraBobAngle( 0.0f )
	, cameraBob( 0.0f )
	, sprint( false )
{
	Translate( Vec3f( -15, -120, -15 ) );

	camera.SetCameraSettings( 45.0f, WIN_ASPECT, 1.0f, 10000.0f );
	camera.SetParent( this );

	move[0] = move[1] = move[2] = move[3] = false;
	jump = false;

	activeItem = ItemDB::GetItem( "Hand" );

	toolbar[0] = "Hand";
	toolbar[1] = "Dirt";
	toolbar[2] = "Stone";
	toolbar[3] = "OBSIDION";
	toolbar[4] = "TNT";
	toolbar[5] = "Wood";
	toolbar[6] = "Diamond";
	toolbar[7] = "GlowStone";

	physicsType = BODY_DYNAMIC;

	InputController::GetInstance().RegisterListener( this );
}

Player::~Player()
{
	InputController::GetInstance().UnRegisterListener( this );
}

void Player::OnButtonDown( Button code )
{
	switch ( code )
	{
		case KEY_W:
		case KEY_w:
			move[0] = true;
			if ( cameraBobAngle >= 359.0f )
				cameraBobAngle = 0.0f;
			else
				cameraBobAngle += 10.f;
			cameraBob = std::sinf( degreesToRadians( cameraBobAngle ) ) / 20.0f;
			break;
		case KEY_S:
		case KEY_s:
			move[1] = true;
			if ( cameraBobAngle <= 1.0f )
				cameraBobAngle = 359.0f;
			else
				cameraBobAngle -= 10.f;
			cameraBob = std::sinf( degreesToRadians( cameraBobAngle ) ) / 20.0f;
			break;
		case KEY_A:
		case KEY_a:
			move[2] = true;
			break;
		case KEY_D:
		case KEY_d:
			move[3] = true;
			break;
		case KEY_SPACE:
			if ( physicsType == BODY_DYNAMIC && !jump )
			{
				velocity.y += 5.0f;
				if ( velocity.y > 20 )
				{
					jumpDelay = 0.4f;
					jump = true;
				}
			}
			else if ( physicsType == BODY_STATIC )
				velocity.y = speed;
			break;
		case KEY_CTRL:
			velocity.y = -speed;
			break;
		case KEY_SHIFT:
			sprint = true;
			break;

		// Toolbar selection
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			activeItem = ItemDB::GetItem( toolbar[ code - '0' - 1 ] );
			break;
	}
}

void Player::OnButtonJustUp( Button code )
{
	switch ( code )
	{
		case KEY_F:
		case KEY_f:
			physicsType = !physicsType;
			break;

		case KEY_W:
		case KEY_w:
			move[0] = false;
			break;
		case KEY_S:
		case KEY_s:
			move[1] = false;
			break;
		case KEY_A:
		case KEY_a:
			move[2] = false;
			break;
		case KEY_D:
		case KEY_d:
			move[3] = false;
			break;
		case KEY_SPACE:
			jump = false;
			break;
		case KEY_SHIFT:
			sprint = false;
			break;
	}
}

void Player::OnButtonJustDown( Button code )
{
}

void Player::OnAxisChanged( Axis which, float x, float y )
{
}

void Player::OnMouseMove( int x, int y ) 
{
	float dx = (x - WIN_WIDTH / 2 ) * 0.2f;
	float dy = (y - WIN_HEIGHT / 2 ) * 0.2f;
	Rotate( Vec3f( dy, dx, 0 ) );
}

void Player::UpdateDynamics( double dt )
{
	static float baseSpeed = 5.0f;
	if ( sprint )
		speed = 2.0f * baseSpeed;
	else
		speed = baseSpeed;

	// Forward movement
	if ( move[0] && move[2] )
		ApplyImpulse( -speed, 135.0f );
	else if ( move[0] && move[3] )
		ApplyImpulse( speed, 45.0f );
	else if ( move[0] )
		ApplyImpulse( speed );
	
	// Backward movement
	else if ( move[1] && move[2] )
		ApplyImpulse( speed, -135.0f );
	else if ( move[1] && move[3] )
		ApplyImpulse( -speed, -45.0f );
	else if ( move[1] )
		ApplyImpulse( -speed );

	// Sideways movement
	else if ( move[2] )
		ApplyImpulse( -speed, 90.0f );
	else if ( move[3] )
		ApplyImpulse( speed, 90.0f );
	else
	{
		cameraBobAngle = 0;
		cameraBob = 0;
	}

	// Apply delay to jumping
	jumpDelay -= (float)dt;
	if ( jumpDelay < 0 && velocity.y == 0 )
		jump = false;

	//position.y += cameraBob;
	// Update physics
	Actor::UpdateDynamics( dt );

	// Update camera
	Vec3f camPos( 0, 0.5f, 0 );
	camPos.y -= cameraBob * ( sprint ? 2.0f : 1.0f );
	camera.SetPosition( camPos );
	//camera.SetPosition( position );
	//camera.CalculateFrustum();

	Transform( position, rotation );
}