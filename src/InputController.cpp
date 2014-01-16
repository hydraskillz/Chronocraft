#include "InputController.h"

#if INPUT_WIN32
#	include <Windows.h>
#	define LEFT_MOUSE_BUTTON VK_LBUTTON
#	define RIGHT_MOUSE_BUTTON VK_RBUTTON
#else if INPUT_GLUT
#	include <glut.h>
#	define LEFT_MOUSE_BUTTON GLUT_LEFT_BUTTON
#	define RIGHT_MOUSE_BUTTON GLUT_RIGHT_BUTTON
#endif

#if INPUT_USE_XINPUT
#	include <Xinput.h>
#	pragma comment (lib, "XInput.lib")
#endif

#include <algorithm>

using namespace mzj;

InputController::InputController()
	: INIT_SINGLETON( InputController )
	, m_LastGamepadPacket( 0 )
	, m_MousePinned( false )
{
	m_CurrentInputState.insert( InputStatePair( KEYBOARD, std::vector< ButtonState >( 256, UP ) ) );
	m_CurrentInputState.insert( InputStatePair( MOUSE,    std::vector< ButtonState >( 3,   UP ) ) );
	m_CurrentInputState.insert( InputStatePair( GAMEPAD,  std::vector< ButtonState >( 28,  UP ) ) );

#if INPUT_USE_XINPUT
	m_GP_XINPUT_STATES[ 0] = XINPUT_GAMEPAD_A;
	m_GP_XINPUT_STATES[ 1] = XINPUT_GAMEPAD_B;
	m_GP_XINPUT_STATES[ 2] = XINPUT_GAMEPAD_X;
	m_GP_XINPUT_STATES[ 3] = XINPUT_GAMEPAD_Y;
	m_GP_XINPUT_STATES[ 4] = XINPUT_GAMEPAD_START;
	m_GP_XINPUT_STATES[ 5] = XINPUT_GAMEPAD_BACK;
	m_GP_XINPUT_STATES[ 6] = XINPUT_GAMEPAD_RIGHT_THUMB;
	m_GP_XINPUT_STATES[ 7] = XINPUT_GAMEPAD_LEFT_THUMB;
	m_GP_XINPUT_STATES[ 8] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	m_GP_XINPUT_STATES[ 9] = XINPUT_GAMEPAD_LEFT_SHOULDER;
	m_GP_XINPUT_STATES[10] = XINPUT_GAMEPAD_DPAD_DOWN;
	m_GP_XINPUT_STATES[11] = XINPUT_GAMEPAD_DPAD_UP;
	m_GP_XINPUT_STATES[12] = XINPUT_GAMEPAD_DPAD_LEFT;
	m_GP_XINPUT_STATES[13] = XINPUT_GAMEPAD_DPAD_RIGHT;
#endif

	for ( int i = 0; i < 236; ++i ) m_keyStates[ i ] = false;
}

void InputController::RegisterListener( InputListener* listener )
{
	m_ListenerList.push_back( listener );
}

void InputController::UnRegisterListener( InputListener* listener )
{
	m_ListenerList.remove( listener );
}

inline ButtonState InputController::DecodeKeyState( int key )
{
#if INPUT_WIN32
	int keyState;
	keyState = ::GetKeyState( key );
	return ( keyState & 0x8000 ) ? DOWN : UP;
#else
	return m_keyStates[ key ] ? DOWN : UP;
#endif
}

ButtonState InputController::GetNewState( ButtonState oldState, ButtonState newState )
{
	if ( oldState == newState )
		return oldState;

	// Just went down
	if ( ( oldState & UP ) && ( newState & DOWN ) )
		return JUST_DOWN;

	// Just went up
	if ( ( oldState & DOWN ) && ( newState & UP ) )
		return JUST_UP;

	return oldState;
}

void InputController::UpdateInput()
{
	// Update button states
	std::for_each( m_CurrentInputState.begin(), m_CurrentInputState.end(), [&]( InputStatePair is )
	{
		switch ( is.first )
		{
		case KEYBOARD:
			for ( int i = 0; i < 256; ++i )
			{
				// Just went down
				if ( is.second[ i ] & JUST_DOWN )
				{
					m_CurrentInputState[ KEYBOARD ][ i ] = DOWN;
				}

				// Just went up
				else if ( is.second[ i ] & JUST_UP )
				{
					m_CurrentInputState[ KEYBOARD ][ i ] = UP;
				}
			}
			break;
		case GAMEPAD:
			for ( int i = 0; i < 28; ++i )
			{
				// Just went down
				if ( is.second[ i ] & JUST_DOWN )
				{
					m_CurrentInputState[ GAMEPAD ][ i ] = DOWN;
				}

				// Just went up
				else if ( is.second[ i ] & JUST_UP )
				{
					m_CurrentInputState[ GAMEPAD ][ i ] = UP;
				}
			}
			break;
		}
	});
		
	// Get new button states
	std::for_each( m_CurrentInputState.begin(), m_CurrentInputState.end(), [&]( InputStatePair is )
	{
		switch ( is.first )
		{
		case KEYBOARD:
			for ( int i = 0; i < 256; ++i )
			{
				ButtonState newState = DecodeKeyState( i );
				m_CurrentInputState[ KEYBOARD ][ i ] = GetNewState( is.second[ i ], newState );

				if ( m_CurrentInputState[ KEYBOARD ][ i ] & JUST_DOWN )
					std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
					{
						il->OnButtonJustDown( (Button) i );
					});
				else if ( m_CurrentInputState[ KEYBOARD ][ i ] & JUST_UP )
					std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
					{
						il->OnButtonJustUp( (Button) i );
					});
				else if ( ( is.second[ i ] & DOWN ) && ( newState & DOWN ) )
					std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
					{
						il->OnButtonDown( (Button) i );
					});
			}
			break;
		case MOUSE:
			{
			ButtonState L = DecodeKeyState( LEFT_MOUSE_BUTTON );
			ButtonState R = DecodeKeyState( RIGHT_MOUSE_BUTTON );

			m_CurrentInputState[ MOUSE ][ 0 ] = GetNewState( m_CurrentInputState[ MOUSE ][ 0 ], L );
			m_CurrentInputState[ MOUSE ][ 1 ] = GetNewState( m_CurrentInputState[ MOUSE ][ 1 ], R );
			}
			break;
		case GAMEPAD:
			{
#if INPUT_USE_XINPUT
			for ( int j = 0; j < 2; ++j )
			{
				DWORD dwResult;
				XINPUT_STATE state;

				ZeroMemory( &state, sizeof( XINPUT_STATE) );

				dwResult = XInputGetState( j, &state );

				if ( dwResult == ERROR_SUCCESS )
				{
					if ( state.dwPacketNumber != m_LastGamepadPacket )
					{
						m_LastGamepadPacket = state.dwPacketNumber;

						for ( int i = 14 * j, k = 0; i < 14 * ( j + 1 ); ++i, ++k )
						{
							ButtonState newState = ( state.Gamepad.wButtons & m_GP_XINPUT_STATES[k] ) ? DOWN : UP;
							m_CurrentInputState[ GAMEPAD ][ i ] = GetNewState( is.second[ i ], newState );

							if ( m_CurrentInputState[ GAMEPAD ][ i ] & JUST_DOWN )
								std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
								{
									il->OnButtonJustDown( (Button) i );
								});
							else if ( m_CurrentInputState[ GAMEPAD ][ i ] & JUST_UP )
								std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
								{
									il->OnButtonJustUp( (Button) i );
								});
							else if ( ( is.second[ i ] & DOWN ) && ( newState & DOWN ) )
								std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
								{
									il->OnButtonDown( (Button) i );
								});
						}

						// Joystick
						float x = state.Gamepad.sThumbLX;
						float y = state.Gamepad.sThumbLY;

						float len = std::sqrtf( x*x + y*y );

						float norm = 0.0f;

						if ( len > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
						{
							if ( len > 32767 ) len = 32767;

							len -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

							norm = len / ( 32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
						}
						else
						{
							len = 0.0f;
						}

						if ( len != 0.0f )
						{
							x = ( x / len );
							y = ( y / len );
						}
						else
						{
							x = 0.0f;
							y = 0.0f;
						}
						std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
						{
							il->OnAxisChanged( Axis(2*j), x, y );
						});
					}
				}
				}
#endif
			}
			break;
		}
	});
}

void InputController::UpdateMouse( int x, int y )
{
	std::for_each( m_ListenerList.begin(), m_ListenerList.end(), [&]( InputListener* il )
	{
		il->OnMouseMove( x, y );
	});
}

void InputController::UpdateKeyState( unsigned char key, bool state )
{
	m_keyStates[ key ] = state;
}