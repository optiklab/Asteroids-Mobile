
#include "BaseEnemy.h"
#include "Common.h"

namespace asteroids
{
	BaseEnemy::BaseEnemy()
	{
		_angle = 0;
		_distance = 0;
		_x = 0;
		_y = 0;
		_movementType = 0;
	}

	float BaseEnemy::_GetInitialDistance(float angle, float goalX, float goalY, float width, float height)
	{
		float defaultMaxDistance = sqrt(2*pow(ORTHO_MAX_D_Y/2, 2));

		float x = goalX + defaultMaxDistance * cos(angle * MATH_PI / 180);
		float y = goalY + defaultMaxDistance * sin(angle * MATH_PI / 180);

		if (x > width)
			x = x - abs(x - width) + 30;
		else if (x < 0)
			x = -30;

		if (y > width)
			y = y - abs(y - height) + 30;
		else if (y < 0)
			y = -30;

		return sqrt(pow(abs(x - goalX), 2) + pow(abs(y - goalY), 2));
	}

	float BaseEnemy::_GetRandomAngle()
	{
		return _GetRandomNumber(360);
	}

	int BaseEnemy::_GetRandomTypeOfMovement()
	{
		return (int)_GetRandomNumber(3); // 3 types of movement
	}

	float BaseEnemy::_GetRandomNumber(int max)
	{
		static int inc = 10;
		srand(time(NULL) - inc);

		float result = rand() % max;

		inc -= result * 10;

		return result;
	}

	float BaseEnemy::GetX()
	{
		return _x;
	}

	float BaseEnemy::GetY()
	{
		return _y;
	}
}
