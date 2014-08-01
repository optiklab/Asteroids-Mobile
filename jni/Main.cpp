#include "Game.h"
#include "Log.h"

void android_main(android_app* pApplication)
{
	asteroids::Game game(pApplication);
	game.NewGame();
}
