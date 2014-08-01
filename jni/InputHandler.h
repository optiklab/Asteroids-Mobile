#pragma once
#ifndef __inputhandler_h__
#define __inputhandler_h__

#include <android/sensor.h>
#include <android/input.h>

namespace asteroids
{
// Handles events from different sources.
	class InputHandler
	{
	public:
		virtual ~InputHandler() {};
		virtual bool OnTouchEvent(AInputEvent* pEvent) = 0;
		virtual bool OnKeyboardEvent(AInputEvent* pEvent) = 0;
		virtual bool OnTrackballEvent(AInputEvent* pEvent) = 0;
		virtual bool OnAccelerometerEvent(ASensorEvent* pEvent) = 0;
	};
}

#endif /* __inputhandler_h__ */

