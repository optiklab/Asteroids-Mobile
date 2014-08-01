
#include "EnemyDust.h"
#include "Common.h"

#include <GLES/gl.h>
#include <GLES/glext.h>

namespace asteroids
{
	EnemyDust::EnemyDust()
	{
		_angle = _GetRandomAngle();
		_cosA = cos(_angle * MATH_PI / 180);
		_sinA = sin(_angle * MATH_PI / 180);

		_distance = _GetInitialDistance(_angle, ORTHO_MAX_D_X/2, ORTHO_MAX_D_Y/2, ORTHO_MAX_D_X, ORTHO_MAX_D_Y);
		_x = ORTHO_MAX_D_X/2 + _distance * cos(_angle * MATH_PI / 180);
		_y = ORTHO_MAX_D_Y/2 + _distance * sin(_angle * MATH_PI / 180);

		_movementType = _GetRandomTypeOfMovement();

		if (_movementType == 0)
		{
			_deltaX = ORTHO_MAX_D_X/2;
			_deltaY = ORTHO_MAX_D_Y/2;
		}
		else if (_movementType == 1)
		{
			_deltaX = ORTHO_MAX_D_X/3;
			_deltaY = ORTHO_MAX_D_Y/3;
		}
		else
		{
			_deltaX = ORTHO_MAX_D_X/3;
			_deltaY = ORTHO_MAX_D_Y/3;
		}
	}

	EnemyDust::~EnemyDust()
	{
	}

	void EnemyDust::Draw()
	{
		glColor4f(1.0, 0.0, 1.0, 1.0);
		GLfloat vectors[] = { _x, _y };
		glVertexPointer(1, GL_FLOAT, 0, vectors);
		glDrawArrays(GL_POINTS, 0, 1);
	}

	void EnemyDust::Update(int time, int total)
	{
		_distance -= time * 0.01;

		_x = _deltaX + _distance * _cosA;
		_y = _deltaY + _distance * _sinA;
	}

	float EnemyDust::GetOuterRadius()
	{
		return 1;
	}
}
