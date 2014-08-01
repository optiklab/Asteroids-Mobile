#include "Game.h"
#include "Log.h"

#include <string.h>

namespace asteroids
{
	Game::Game(android_app* pApplication) : _sensorPollSource()
	{
		_pApplication = pApplication;

		_pApplication->userData = this;
		_pApplication->onAppCmd = _CallbackEvent;
		_pApplication->onInputEvent = _CallbackInput;

		_enabled = false;
		_quit = false;

		_width = 0;
		_height = 0;
		_display = EGL_NO_DISPLAY;
		_surface = EGL_NO_CONTEXT;
		_context = EGL_NO_SURFACE;

		// TODO Implement work with accelerometer.
		_pSensorManager = NULL;
		_pSensorEventQueue = NULL;
		_sensorType = ASENSOR_TYPE_ACCELEROMETER;
		//_pSensor = NULL;
	}

	Game::~Game()
	{
		if (_pController != 0)
		{
			delete _pController;
			_pController = 0;
		}
	}

	// Creates new game (totally new or restart), supports infinite game loop and quit.
	void Game::NewGame()
	{
		int32_t lResult;
		int32_t lEvents;
		android_poll_source* lSource;

		// Makes sure native glue is not stripped by the linker.
		app_dummy();

		// Global step loop.
		while (true)
		{
			// Event processing loop.
			while ((lResult = ALooper_pollAll(_enabled ? 0 : -1, NULL, &lEvents, (void**) &lSource)) >= 0)
			{
				// An event has to be processed.
				if (lSource != NULL)
				{
					lSource->process(_pApplication, lSource);
				}
				// Application is getting destroyed.
				if (_pApplication->destroyRequested)
				{
					return;
				}
			}

			// Steps the application.
			if ((_enabled) && (!_quit))
			{
				status result = _pController->OnStep();

				if (result == STATUS_EXIT || result == STATUS_KO) // Exit game.
				{
					Log::info("quit");
					_quit = true;
					ANativeActivity_finish(_pApplication->activity);
				}
				else if (result == STATUS_RESTART) // Restart game.
				{
					Log::info("_restart");

					_HaltGame();
					_enabled = false;

					if (_pController != 0)
					{
						delete _pController;
						_pController = 0;
					}

					Activate();
					NewGame();
					break;
				}
			}
		}
	}

	// Handles activate event.
	void Game::Activate()
	{
		//Log::info("Activate");

		// Enables activity only if a window is available.
		if ((!_enabled) && (_pApplication->window != NULL))
		{
			_quit = false;
			_enabled = true;

			// Initialize sensors
			_sensorPollSource.id = LOOPER_ID_USER;
			_sensorPollSource.app = _pApplication;
			_sensorPollSource.process = _CallbackSensor;
			_pSensorManager = ASensorManager_getInstance();
			if (_pSensorManager != NULL)
			{
				_pSensorEventQueue = ASensorManager_createEventQueue(_pSensorManager, _pApplication->looper,
						LOOPER_ID_USER, NULL, &_sensorPollSource);

				//Log::info("ASensorManager_createEventQueue");

				if (_pSensorEventQueue == NULL)
				{
					_quit = true;

					return;
				}
			}

			_InitDisplay();

			_pController = new GameController(_display, _surface, _width, _height); // TODO

			if (_pController->OnActivate() != STATUS_OK)
			{
				_quit = true;
				Deactivate();
				ANativeActivity_finish(_pApplication->activity);
			}
		}
	}

	// Handles deactivate event.
	void Game::Deactivate()
	{
		if (_enabled)
		{
			_HaltGame();
			_DisableSensor();
			_pController->OnDeactivate();
			_enabled = false;

			if (_pSensorEventQueue != NULL)
			{
				ASensorManager_destroyEventQueue(_pSensorManager, _pSensorEventQueue);
				_pSensorEventQueue = NULL;
			}
			_pSensorManager = NULL;
		}
	}

	//
	void Game::ProcessAppEvent(int32_t pCommand)
	{
		switch (pCommand)
		{
	//    case APP_CMD_CONFIG_CHANGED:
	//        _pController->onConfigurationChanged();
	//        break;
	//    case APP_CMD_INIT_WINDOW:
	//    	_pController->onCreateWindow();
	//        break;
	//    case APP_CMD_DESTROY:
	//    	_pController->onDestroy();
	//        break;
		case APP_CMD_GAINED_FOCUS:
			Activate();
		   // _pController->onGainFocus();
			break;
		case APP_CMD_LOST_FOCUS:
			//_pController->onLostFocus();
			Deactivate();
			break;
	//    case APP_CMD_LOW_MEMORY:
	//    	_pController->onLowMemory();
	//        break;
		case APP_CMD_PAUSE:
			//_pController->onPause();
			Deactivate();
			break;
	//    case APP_CMD_RESUME:
	//    	_pController->onResume();
	//        break;
	//    case APP_CMD_SAVE_STATE:
	//    	_pController->onSaveState(&_pApplication->savedState, &_pApplication->savedStateSize);
	//        break;
	//    case APP_CMD_START:
	//    	_pController->onStart();
	//        break;
	//    case APP_CMD_STOP:
	//    	_pController->onStop();
	//        break;
		case APP_CMD_TERM_WINDOW:
			//_pController->onDestroyWindow();
			Deactivate();
			break;
		default:
			break;
		}
	}

	int32_t Game::ProcessInputEvent(AInputEvent* pEvent)
	{
		int32_t lEventType = AInputEvent_getType(pEvent);
		switch (lEventType)
		{
		case AINPUT_EVENT_TYPE_MOTION:

			switch (AInputEvent_getSource(pEvent))
			{
				case AINPUT_SOURCE_TOUCHSCREEN:
					return _pController->OnTouchEvent(pEvent);
					break;
				case AINPUT_SOURCE_TRACKBALL:
					return _pController->OnTrackballEvent(pEvent);
					break;
			}

			break;
		case AINPUT_EVENT_TYPE_KEY:
			//TODO Accelerometer
			_ToggleSensor();
			return _pController->OnKeyboardEvent(pEvent);
			break;
		}
		return 0;
	}

	//
	void Game::ProcessSensorEvent()
	{
		//Log::info("ProcessSensorEvent");

		ASensorEvent lEvent;
		while (ASensorEventQueue_getEvents(_pSensorEventQueue, &lEvent, 1) > 0)
		{
			switch (lEvent.type)
			{
			case ASENSOR_TYPE_ACCELEROMETER:
				//Log::info("ASENSOR_TYPE_ACCELEROMETER");

				_pController->OnAccelerometerEvent(&lEvent);
				break;
			}
		}
	}

	int Game::_InitDisplay()
	{
		EGLint lFormat, lNumConfigs, lErrorResult;
		EGLConfig lConfig;

		// Defines display requirements. 16bits mode here.
		const EGLint lAttributes[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
			EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_NONE
		};

		// Retrieves a display connection and initializes it.
		_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		if (_display != EGL_NO_DISPLAY &&
			eglInitialize(_display, NULL, NULL) &&
			// Selects the first OpenGL configuration found.
			eglChooseConfig(_display, lAttributes, &lConfig, 1, &lNumConfigs) && (lNumConfigs > 0) &&
			// Reconfigures the Android window with the EGL format.
			eglGetConfigAttrib(_display, lConfig, EGL_NATIVE_VISUAL_ID, &lFormat))
		{
			ANativeWindow_setBuffersGeometry(_pApplication->window, 0, 0, lFormat);

			// Creates the display surface.
			_surface = eglCreateWindowSurface(_display, lConfig, _pApplication->window, NULL);

			if (_surface != EGL_NO_SURFACE)
			{
				_context = eglCreateContext(_display, lConfig, EGL_NO_CONTEXT, NULL);

				// Activates the display surface.
				if (_context != EGL_NO_CONTEXT &&
					eglMakeCurrent(_display, _surface, _surface, _context) &&
					eglQuerySurface(_display, _surface, EGL_WIDTH, &_width) &&
					eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height) &&
					(_width > 0) && (_height > 0))
				{
					glViewport(0, 0, _width, _height);

					// Displays information about OpenGL.
					Log::info("Version  : %s", glGetString(GL_VERSION));
					Log::info("Vendor   : %s", glGetString(GL_VENDOR));
					Log::info("Renderer : %s", glGetString(GL_RENDERER));
					Log::info("Viewport : %d x %d", _width, _height);

					return STATUS_OK;
				}
			}
		}

		_HaltGame();

		return STATUS_KO;
	}

	// Clears OpenGL resources.
	bool Game::_HaltGame()
	{
		// Destroys OpenGL context.
		if (_display != EGL_NO_DISPLAY)
		{
			eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (_context != EGL_NO_CONTEXT)
			{
				eglDestroyContext(_display, _context);
				_context = EGL_NO_CONTEXT;
			}
			if (_surface != EGL_NO_SURFACE)
			{
				eglDestroySurface(_display, _surface);
				_surface = EGL_NO_SURFACE;
			}
			eglTerminate(_display);
			_display = EGL_NO_DISPLAY;

			Log::info("HaltGame");
		}

		return true;
	}

	// Private callbacks handling events occuring in the thread loop.
	void Game::_CallbackEvent(android_app* pApplication, int32_t pCommand)
	{
		Game& game = *(Game*) pApplication->userData;
		game.ProcessAppEvent(pCommand);
	}

	// Handles input events.
	int32_t Game::_CallbackInput(android_app* pApplication, AInputEvent* pEvent)
	{
		Game& game = *(Game*) pApplication->userData;
		return game.ProcessInputEvent(pEvent);
	}

	// Handles sensor events.
	void Game::_CallbackSensor(android_app* pApplication, android_poll_source* pSource)
	{
		Game& game = *(Game*) pApplication->userData;
		game.ProcessSensorEvent();
	}

	status Game::_ToggleSensor()
	{
		return true;//(_pSensor != NULL) ? _DisableSensor() : _EnableSensor();
	}

	status Game::_EnableSensor()
	{
		if (_enabled)
		{
//			_pSensor = ASensorManager_getDefaultSensor(_pSensorManager, _sensorType);
//
//			if (_pSensor != NULL)
//			{
//				if (ASensorEventQueue_enableSensor(_pSensorEventQueue, _pSensor) >= 0)
//				{
//					int32_t lMinDelay = ASensor_getMinDelay(_pSensor);
//
//					if (ASensorEventQueue_setEventRate(_pSensorEventQueue, _pSensor, lMinDelay) >= 0)
//					{
//						return STATUS_OK;
//					}
//				}
//
//				_DisableSensor();
//				return STATUS_KO;
//			}
		}

		return STATUS_OK;

	}

	status Game::_DisableSensor()
	{
//		if (_enabled && (_pSensor != NULL))
//		{
//			if (ASensorEventQueue_disableSensor(_pSensorEventQueue, _pSensor) < 0)
//			{
//				return STATUS_KO;
//			}
//
//			_pSensor = NULL;
//		}

		return STATUS_OK;
	}
}
