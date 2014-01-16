/*
 * Author      : Matthew Johnson
 * Date        : 18/Sep/2012
 * Description :
 *   Interface for controlling input and firing callbacks.
 */
#pragma once

#include "Singleton.h"
#include "InputListener.h"

#define INPUT_USE_XINPUT 0
#define INPUT_WIN32 1
#define INPUT_GLUT 0

#include <map>
#include <list>
#include <vector>

namespace mzj
{
	typedef std::pair< InputDevice, std::vector< ButtonState >> InputStatePair;
	typedef std::pair< float, float > AxisVector;

	class InputController
		: public Singleton< InputController >
	{
	public:
		InputController();

		void RegisterListener( InputListener* listener );
		void UnRegisterListener( InputListener* listener );

		void UpdateInput();
		void UpdateMouse( int x, int y );
		void UpdateKeyState( unsigned char key, bool state );

		void PinMouse( bool pin ) { m_MousePinned = pin; }
		bool IsMousePinned() const { return m_MousePinned; }

	private:
		ButtonState GetNewState( ButtonState oldState, ButtonState newState );
		inline ButtonState DecodeKeyState( int key );

		std::list< InputListener* > m_ListenerList;
		std::map< InputDevice, std::vector< ButtonState > > m_CurrentInputState;

		unsigned long m_LastGamepadPacket;
		unsigned long m_GP_XINPUT_STATES[14];

		bool m_keyStates[256];

		bool m_MousePinned;
	};

}