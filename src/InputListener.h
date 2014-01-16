/*
 * Author      : Matthew Johnson
 * Date        : 18/Sep/2012
 * Description :
 *   Interface for class that want to listen for input.
 *   Must register/unregister via InputController in their implementations.
 */
#pragma once

#include "Input.h"

namespace mzj
{
	
	class InputListener
	{
	public:
		InputListener() {}

		virtual void OnButtonDown    ( Button code ){}
		virtual void OnButtonJustUp  ( Button code ){}
		virtual void OnButtonJustDown( Button code ){}

		virtual void OnAxisChanged( Axis which, float x, float y ){}

		virtual void OnMouseMove( int dx, int dy ) {}
	};

}