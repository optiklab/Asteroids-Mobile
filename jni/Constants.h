#pragma once
#ifndef __constants_h__
#define __constants_h__

#include <stdint.h>
#include <string>

const float MATH_PI = 3.14159265358979323846;

const std::string GAME_NAME = "ASTEROIDS";
const std::string POINTS_LABEL = "Points: ";
const std::string LEVEL_LABEL = "Level: ";
const std::string LIVES_LABEL = " lives";

typedef int status;

const status STATUS_OK = 0;
const status STATUS_KO = -1;
const status STATUS_EXIT = -2;
const status STATUS_RESTART = -3;

const int ORTHO_MAX_X = 480;
const int ORTHO_MAX_Y = 320;
const float ORTHO_MAX_D_X = 480.0;
const float ORTHO_MAX_D_Y = 320.0;

const float INPUT_BUTTON_SIZE = 80.0;

const int MAX_BULLETS = 10;
const float SIDE_MOVING_ACC = 0.1;

const int PENTAGON_RIBERS_NUMBER = 5;
const float DEFAULT_ASTEROID_RADIUS = 15;

const float PENTAGON_TRIANGLE_ANGLES = 72;

const int TIME_BOMBS_ALIVE = 2;
const int TIME_INITIAL_SCREEN_ALIVE = 4;
const float DEFAULT_ENEMY_APPEAR_INTERVAL = 2.0f;
const float GAME_STOP_INTERVAL = 3.0f;

const float ENEMY_STEP = 30.0f;

const int DEFAULT_LIVES_COUNT = 5;


#endif /* __constants_h__ */
