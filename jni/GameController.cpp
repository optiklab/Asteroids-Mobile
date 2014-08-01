#include <string>
#include <streambuf>
#include <iostream>
#include <sstream>

#include <GLES/gl.h>
#include <GLES/glext.h>

// TODO Use Freetype for cool fonts in game.
//#include <ft2build.h>
//
//#include FT_FREETYPE_H
//#define CHAR_SIZE 16
//#define SLOT_SIZE 32
//#define HEIGHT CHAR_SIZE*SLOT_SIZE
//unsigned char image[HEIGHT][WIDTH][4];

#include "GameController.h"
#include "Common.h"
#include "DrawHelpers.h"
#include "Log.h"

namespace asteroids
{
	// Constructor.
	GameController::GameController(EGLDisplay display, EGLSurface surface, int32_t width, int32_t height)
	{
		_pTimeService = new TimeService();

		_display = display;
		_surface = surface;

		_lastAppearTime = 0;
		_lastTimeAfterKill = 0;

		_isGameState = false;
		_isScoreState = false;

		_quit = false;
		_restart = false;

		_width = width;
		_height = height;

		_player = new Player();
		_statistics = new Statistics(_width, _height);
	}

	// Clears resources.
	GameController::~GameController()
	{
		if (_pTimeService != 0)
		{
			delete _pTimeService;
			_pTimeService = 0;
		}
	}

	// Determines is game initialized.
	bool GameController::IsInitialized()
	{
		if (_isGameState)
		{
			return true;
		}

		return false;
	}

	// Handles when application activates.
	status GameController::OnActivate()
	{
		// Initializes base GL state.
		glEnable(GL_TEXTURE_2D); // NEW
		// In a simple 2D game, we have control over the third
		// dimension. So we do not really need a Z-buffer.
		glDisable(GL_DEPTH_TEST); // NEW

		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glShadeModel(GL_FLAT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrthof(0, ORTHO_MAX_X, 0, ORTHO_MAX_Y, -1.0, 1.0);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();

		glPointSize(3.0);

		_pTimeService->Reset();

		return STATUS_OK;
	}

	void GameController::OnDeactivate()
	{
		// TODO ?
	}

	status GameController::OnStep()
	{
		_pTimeService->Update();

		_Update();

		if (_quit)
		{
			return STATUS_EXIT;
		}
		else if (_restart)
		{
			return STATUS_RESTART;
		}

		return STATUS_OK;
	}


	bool GameController::OnTouchEvent(AInputEvent* pEvent)
	{
		size_t count = AMotionEvent_getPointerCount(pEvent);

		float total = _pTimeService->TotalElapsed();
		float time = _pTimeService->Elapsed();

		int32_t action = AMotionEvent_getAction(pEvent);

		for (int i = 0; i < count; i++)
		{
			int32_t id = AMotionEvent_getPointerId(pEvent, i);

			float x = -1;
			float y = -1;

			switch (action & AMOTION_EVENT_ACTION_MASK)
			{
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
				id = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				// NO BREAK!
			case AMOTION_EVENT_ACTION_MOVE:
			case AMOTION_EVENT_ACTION_DOWN:
				x = AMotionEvent_getX(pEvent, id);
				y = AMotionEvent_getY(pEvent, id);
				//Log::info("touch %f, %f", x, y);

				// If we show score (ship already destroyed)... we need to handle buttons RESTART and EXIT.
				if (_isScoreState &&
					y >= _height/3.0 && y <= _height - _height/3.0)
				{
					//Log::info("x %d", x);

					if (x <= _width/2.0)
					{
						//Log::info("restart");
						_restart = true;
					}
					else if (x >= _width/2.0)
					{
						//Log::info("_quit");
						_quit = true;
					}
				}
				else
				{
					_player->Touch(id, total, x, y);
				}
				break;
			case AMOTION_EVENT_ACTION_POINTER_UP:
				id = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				//Log::info("AMOTION_EVENT_ACTION_POINTER_DOWN %d", id);
				// NO BREAK!
			case AMOTION_EVENT_ACTION_UP:
				x = AMotionEvent_getX(pEvent, id);
				y = AMotionEvent_getY(pEvent, id);
				//Log::info("untouch %f, %f", x, y);
				_player->Untouch(id, total, x, y);

				break;
			}
		}

		return true;
	}

	bool GameController::OnKeyboardEvent(AInputEvent* pEvent)
	{
		return true;
	}

	bool GameController::OnTrackballEvent(AInputEvent* pEvent)
	{
		return true;
	}

	//TODO Accelerometer
	bool GameController::OnAccelerometerEvent(ASensorEvent* pEvent)
	{
		const float GRAVITY = ASENSOR_STANDARD_GRAVITY / 2.0f;
		const float MIN_X = -1.0f;
		const float MAX_X = 1.0f;
		const float MIN_Y = 0.0f;
		const float MAX_Y = 2.0f;
		const float CENTER_X = (MAX_X + MIN_X) / 2.0f;
		const float CENTER_Y = (MAX_Y + MIN_Y) / 2.0f;
		float lRawHorizontal = pEvent->vector.x / GRAVITY;

		if (lRawHorizontal > MAX_X)
		{
			lRawHorizontal = MAX_X;
		}
		else if (lRawHorizontal < MIN_X)
		{
			lRawHorizontal = MIN_X;
		}

		_horizontal = CENTER_X - lRawHorizontal;
		float lRawVertical = pEvent->vector.z / GRAVITY;

		if (lRawVertical > MAX_Y)
		{
			lRawVertical = MAX_Y;
		}
		else if (lRawVertical < MIN_Y)
		{
			lRawVertical = MIN_Y;
		}

		_vertical = lRawVertical - CENTER_Y;

		//Log::info("x: %d y: %d z: %d", pEvent->vector.x, pEvent->vector.y, pEvent->vector.z);

		return true;
	}

	// Updates all the game logic and draws all parts of the game.
	void GameController::_Update()
	{
		float total = _pTimeService->TotalElapsed();
		float time = _pTimeService->Elapsed();

		// Clears background color.
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (_isGameState)
		{
			_player->Update(time, total);

			_UpdateAsteroids(time, total);

			// DRAWS ALL!
			_player->Draw();

			int size = _asteroids.size();
			for (int i = 0; i < size; i++)
			{
				_asteroids[i]->Draw();
			}

			size = _dust.size();
			for (int i = 0; i < size; i++)
			{
				_dust[i]->Draw();
			}

			_statistics->Draw();
			_Draw();

			if (_isScoreState)
			{
				DrawHelpers::DrawRestartQuestion();
			}
		}
		else
		{
			// Show initial screen with game title during some time...
			if (total <= TIME_INITIAL_SCREEN_ALIVE)
			{
				_ShowInitialScreen();
			}
			else
			{
				// Start game.
				_isGameState = true;
			}
		}

		// Shows rendering surface.
		if (eglSwapBuffers(_display, _surface) != EGL_TRUE)
		{
			//Log::error("Error %d swapping buffers.", eglGetError());
			//return;
		}

		if (!_player->IsKilled())
		{
			_SolveCollisions();

			if (_player->IsKilled())
			{
				_lastTimeAfterKill = total;
			}
		}
		else if ((total - _lastTimeAfterKill) > GAME_STOP_INTERVAL)
		{
			_isScoreState = true;
		}
	}

	// Draws game controller itself.
	void GameController::_Draw()
	{
		DrawHelpers::DrawGunButton();
		DrawHelpers::DrawLeftButton();
		DrawHelpers::DrawRightButton();
		DrawHelpers::DrawMovingButton();
	}

	// Solves all collisions between player, asteroids and rockets.
	void GameController::_SolveCollisions()
	{
		int size = _asteroids.size();
		for (int i = 0; i < size; i++)
		{
			AsteroidEnemy* enemy = _asteroids[i];

			if (!enemy->HasEffectiveDamage())
			{
				continue;
			}

			// Bullets with asteroids
			bool isHit = _player->IsHit(enemy);
			// Player with asteroids
			bool isPlayerDamaged = _player->IsDamagedByEnemy(enemy);

			// Asteroids with asteroids
			// It was not implemented in the actual Asteroids. So I don't know if need this.

			// Split enemy to pieces.
			if (isHit || isPlayerDamaged)
			{
				std::vector<AsteroidEnemy*> result = enemy->SplitToPieces();

				_asteroids[i]->SetVisible(false); // This asteroid will be removed in Update.

				int addedCount = result.size();
				for (int j = 0; j < addedCount; j++)
				{
					_asteroids.push_back(result[j]);
				}
			}

			if (isPlayerDamaged)
			{
				_statistics->DecreaseLives();

				// Set player killed.
				if (_statistics->GetLivesNumber() == 0)
				{
					_player->SetKilled(true);
				}
			}

			if (isHit)
			{
				_statistics->IncreasePointsEarned();
			}
		}
	}

	// Shows initial screen.
	void GameController::_ShowInitialScreen()
	{
		DrawHelpers::DrawGameName();
	}

	// Updates logic related to asteroids.
	void GameController::_UpdateAsteroids(float time, float total)
	{
		// Update, Delete, Add asteroids
		int size = _asteroids.size();
		for (int i = 0; i < size; i++)
		{
			_asteroids[i]->Update(time);
		}

		for (int i = size - 1; i >= 0; i--)
		{
			if (!_asteroids[i]->IsVisibleInViewport() || !_asteroids[i]->IsVisible())
			{
				std::vector<AsteroidEnemy*>::iterator p = _asteroids.begin();
				p += i;

				delete _asteroids[i];
				_asteroids.erase(p);
			}
		}

		if ((total - _lastAppearTime) > DEFAULT_ENEMY_APPEAR_INTERVAL)
		{
			AsteroidEnemy* a = new AsteroidEnemy(_player->GetPosition()->x, _player->GetPosition()->y);
			_asteroids.push_back(a);
			_lastAppearTime = total;
		}

		size = _dust.size();
		for (int i = 0; i < size; i++)
		{
			_dust[i]->Update(time);
		}
	}
}
