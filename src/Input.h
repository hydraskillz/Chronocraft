/*
 * Author      : Matthew Johnson
 * Date        : 18/Sep/2012
 * Description :
 *   Defines for input.
 */
#pragma once

//#define IN_GLUT
#define IN_WIN32

namespace mzj
{
	enum Button
	{
		KEY_NULL   = -1,

		// Keyboard
		KEY_SPACE  = 0x20,
#ifdef IN_WIN32
		KEY_SHIFT  = 0x10,
		KEY_CTRL   = 0x11,
		KEY_ESC    = 0x18,
#else
#	ifdef IN_GLUT
		KEY_SHIFT  = 0x70,
		KEY_CTRL   = 0x72,
		KEY_ALT    = 0x74,
		KEY_ESC    = 0x18,
#	endif
#endif

		KEY_DOWN   = 0x28,
		KEY_UP     = 0x26,
		KEY_LEFT   = 0x25,
		KEY_RIGHT  = 0x27,
		KEY_F4     = 0x73,
		KEY_F3     = 0x72,
		KEY_F2     = 0x71,

		KEY_T      = 'T',
		KEY_G      = 'G',
		KEY_Y      = 'Y',
		KEY_H      = 'H',
		KEY_W	   = 'W',
		KEY_A      = 'A',
		KEY_S      = 'S',
		KEY_D      = 'D',
		KEY_P      = 'P',
		KEY_F      = 'F',
		KEY_L	   = 'L',

		KEY_t      = 't',
		KEY_g      = 'g',
		KEY_y      = 'y',
		KEY_h      = 'h',
		KEY_w	   = 'w',
		KEY_a      = 'a',
		KEY_s      = 's',
		KEY_d      = 'd',
		KEY_p      = 'p',
		KEY_f      = 'f',
		KEY_l	   = 'l',

		KEY_ENTER  = 0x0D,

		KEY_NUM_0  = 0x60,

		// Mouse
#ifdef IN_WIN32
		KEY_LMOUSE = 0x01,
		KEY_RMOUSE = 0x02,
#else
#	ifdef IN_GLUT
		KEY_LMOUSE = 0x00,
		KEY_RMOUSE = 0x02,
#	endif
#endif
		

		// Gamepad
		KEY_GP0_A     = 0x0,
		KEY_GP0_B     = 0x1,
		KEY_GP0_X     = 0x2,
		KEY_GP0_Y     = 0x3,
		KEY_GP0_START = 0x4,
		KEY_GP0_BACK  = 0x5,

		KEY_GP1_A     = 0x0E,
		KEY_GP1_B     = 0x0F,
		KEY_GP1_X     = 0x10,
		KEY_GP1_Y     = 0x11,
		KEY_GP1_START = 0x12,
		KEY_GP1_BACK  = 0x13,
	};

	enum ButtonState
	{
		UP         = 0x0002,
		DOWN       = 0x0004,
		JUST_UP    = 0x0008,
		JUST_DOWN  = 0x0010
	};

	enum Axis
	{
		JOYSTICK0_LEFT  = 0x0,
		JOYSTICK0_RIGHT = 0x1,

		JOYSTICK1_LEFT  = 0x2,
		JOYSTICK1_RIGHT = 0x3
	};
	
	enum InputDevice
	{
		KEYBOARD = 0,
		MOUSE,
		GAMEPAD
	};
}