#pragma once
#ifndef __gamecontroller_h__
#define __gamecontroller_h__

#include <vector>
#include "Player.h"
#include "EnemyDust.h"
#include "AsteroidEnemy.h"
#include "Statistics.h"
#include "ActivityHandler.h"
#include "InputHandler.h"
#include "TimeService.h"


#include <EGL/egl.h>

// TODO
//#include FT_FREETYPE_H

namespace asteroids
{
	// Manage all the game resources and logic.
	class GameController : public ActivityHandler, public InputHandler
	{
	public:
		// Constructor.
		GameController(EGLDisplay display, EGLSurface surface, int32_t width, int32_t height);
		// Clears resources.
		virtual ~GameController();

		// Determines is game initialized.
		bool IsInitialized();

		status OnActivate();
		void OnDeactivate();
		status OnStep();

		bool OnTouchEvent(AInputEvent* pEvent);
		bool OnKeyboardEvent(AInputEvent* pEvent);
		bool OnTrackballEvent(AInputEvent* pEvent);
		bool OnAccelerometerEvent(ASensorEvent* pEvent);

	private:

		// Updates all the game logic and draws all parts of the game.
		void _Update();
		// Draws game controller itself.
		void _Draw();
		// Initializes display to show game.
		void _InitDisplay();
		// Updates logic related to asteroids.
		void _UpdateAsteroids(float time, float total);
		// Solves all collisions between player, asteroids and rockets.
		void _SolveCollisions();
		// Shows initial screen.
		void _ShowInitialScreen();
		// Show number on screen.
		void _ShowNumber(int number);

	private:
		// Player data.
		Player* _player;

		// Some time intervals using for live/visibility time.
		float _lastAppearTime;
		float _lastTimeAfterKill;

		// States control.
		bool _isGameState;
		bool _isScoreState;
		bool _quit;
		bool _restart;

		float _horizontal;
		float _vertical;

		int32_t _width;
		int32_t _height;

		// Display properties.
		EGLDisplay _display;
		EGLSurface _surface;

		Statistics* _statistics;
		// Collection of dust (enemy trash) objects.
		std::vector<EnemyDust*> _dust;
		// Collection of enemies itself: asteroids.
		std::vector<AsteroidEnemy*> _asteroids;

		// Time services to work with.
		TimeService* _pTimeService;
	};
}

#endif /* __gamecontroller_h__ */

