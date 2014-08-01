#pragma once
#ifndef __game_h__
#define __game_h__

#include "Common.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <android_native_app_glue.h>

#include "GameController.h"

namespace asteroids
{
	//
	class Game
	{
	public:
		Game(android_app* pApplication);
		virtual ~Game();

		// Creates new infinite loop for game, controls restart and quit.
		void NewGame();
		// Process application events.
		void ProcessAppEvent(int32_t pCommand);
		// Handles Activity activation.
		void Activate();
		// Handles Activity deactivation: on leave focus, close, etc.
		void Deactivate();
		// Process input events.
		int32_t ProcessInputEvent(AInputEvent* pEvent);
		// Process sensor events.
		void ProcessSensorEvent();

	private:
		// Clears OpenGL resources.
		bool _HaltGame();
		// Initialize display before starting the game.
		int _InitDisplay();
		// Handles events occurring in the thread loop.
		static void _CallbackEvent(android_app* pApplication, int32_t pCommand);
		// Handles input events.
		static int32_t _CallbackInput(android_app* pApplication, AInputEvent* pEvent);
		// Handles sensor events.
		static void _CallbackSensor(android_app* pApplication, android_poll_source* pSource);

		// Manipulating sensor.
		status _ToggleSensor();
		status _EnableSensor();
		status _DisableSensor();

	private:
		// Application details provided by Android.
		android_app* _pApplication;
		// Saves application state when application is active/paused.
		bool _enabled;
		// Indicates if the event handler wants to exit.
		bool _quit;

		// Display properties.
		int32_t _width, _height;
		EGLDisplay _display;
		EGLSurface _surface;
		EGLContext _context;

		// Sensor resources.
		ASensorManager* _pSensorManager;
		ASensorEventQueue* _pSensorEventQueue;

		android_poll_source _sensorPollSource;

		// Controls user inputs, levels, points, lives...manage game process.
		GameController* _pController;

		//TODO Accelerometer
		//const ASensor* _pSensor;
		int32_t _sensorType;
	};
}

#endif /* __game_h__ */
