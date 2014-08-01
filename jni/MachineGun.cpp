
#include "MachineGun.h"
#include "Common.h"
#include "Log.h"

#include <GLES/gl.h>
#include <GLES/glext.h>

namespace asteroids
{
	MachineGun::MachineGun()
	{
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			Bullet b;
			_bullets.push_back(b);
		}
	}

	void MachineGun::Draw()
	{
		glColor4f(1.0, 0.0, 1.0, 1.0);

		size_t count = _bullets.size();

		GLfloat* vectors = new GLfloat[2*count];
		int j = 0;

		for (size_t i=0; i<count; i++)
		{
			if (_bullets[i].inuse &&
				_bullets[i].x > 1.0 && _bullets[i].x < ORTHO_MAX_D_X &&
				_bullets[i].y > 1.0 && _bullets[i].y < ORTHO_MAX_D_Y)
			{
				vectors[j++] = _bullets[i].x;
				vectors[j++] = _bullets[i].y;
			}
		}

		glVertexPointer(2, GL_FLOAT, 0, vectors);
		glDrawArrays(GL_POINTS, 0, count);

		delete[] vectors;
	}

	void MachineGun::Update(float time, float total)
	{
		size_t count = _bullets.size();
		for (size_t i=0; i<count; i++)
		{
			if (_bullets[i].inuse)
			{
				float x = _bullets[i].x + _bullets[i].xv * time;
				float y = _bullets[i].y + _bullets[i].yv * time;
				//x = x / ORTHO_MAX_D_X;
				//_bullets[i].x = (x - floor(x))*ORTHO_MAX_D_X;
				//y = y / ORTHO_MAX_D_Y;
				//_bullets[i].y = (y - floor(y))*ORTHO_MAX_D_Y;

				_bullets[i].x = x;
				_bullets[i].y = y;
				if (total > _bullets[i].expire ||
					_bullets[i].x < 1.0 || _bullets[i].x > ORTHO_MAX_D_X ||
					_bullets[i].y < 1.0 || _bullets[i].y > ORTHO_MAX_D_Y)
				{
					_bullets[i].inuse = 0;
					continue;
				}
			}
		}
	}

	// Creates new bullets.
	void MachineGun::MakeShot(float time, Position playerPosition)
	{
		int bulletId = _GetBulletToShow();
		if (bulletId >= 0)
		{
			_InitBullet(bulletId, time, playerPosition);
		}
	}

	// Which free bullet to select as new.
	int MachineGun::_GetBulletToShow()
	{
		size_t count = _bullets.size();
		for (size_t i=0; i<count; i++)
		{
			if (!_bullets[i].inuse)
			{
				return i;
			}
		}

		return -1;
	}

	// Initialize new bullet.
	void MachineGun::_InitBullet(int i, float time, Position playerPosition)
	{
		float c = cos(playerPosition.angle*MATH_PI/180.0);
		float s = sin(playerPosition.angle*MATH_PI/180.0);

		_bullets[i].inuse = 1;
		_bullets[i].x = playerPosition.x + 2 * c;
		_bullets[i].y = playerPosition.y + 2 * s;
		_bullets[i].v = 80;
		_bullets[i].xv = playerPosition.xv + c * _bullets[i].v;
		_bullets[i].yv = playerPosition.yv + s * _bullets[i].v;
		_bullets[i].expire = time + TIME_BOMBS_ALIVE;
	}

	// Calculates if some bullets damaged objects.
	bool MachineGun::IsDamagedByBullets(AsteroidEnemy* enemy)
	{
		if (enemy != 0)
		{
			size_t count = _bullets.size();
			for (size_t i=0; i<count; i++)
			{
				if (_bullets[i].inuse == 1)
				{
					float enemyX = enemy->GetX();
					float enemyY = enemy->GetY();

					// Find intersection
					float r = sqrt(pow(abs(_bullets[i].x - enemyX), 2) + pow(abs(_bullets[i].y - enemyY), 2));

					if (r <= enemy->GetOuterRadius())
					{
						_bullets[i].inuse = 0;

						 return true;
					}
				}
			}
		}

		return false;
	}
}
