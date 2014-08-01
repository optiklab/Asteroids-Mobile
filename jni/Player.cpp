
#include "Player.h"
#include "Common.h"
#include "Log.h"

namespace asteroids
{
	// Ship turned to the right by default.
	const GLfloat Player::_vertices[] = {
			10.0, 0.0,
			-5.0, -5.0,
			-2.0, 0.0,
			-5.0, 5.0,
			10.0, 0.0
		};

	const GLfloat Player::_thrustVectors[] = {
			-3.5, 2.5,
			-10, 0.0,
			-3.5, -2.5,
		};

	Player::Player()
	{
		_isKilled = false;

		// Default weapon.
		_machineGun = new MachineGun();

		_position.x = ORTHO_MAX_X / 2;
		_position.y = ORTHO_MAX_Y / 2;

		_left = 0;
		_leftTime = 0;
		_right = 0;
		_rightTime = 0;
		_thrust = 0;
		_thrustTime = 0;
		_back = 0;
		_backTime = 0;

		_position.xv = 0;
		_position.yv = 0;

		_position.angle = 0.0;

		_killedDelta = 1.0;
	}

	void Player::Draw()
	{
		glPushMatrix();
		glTranslatef(_position.x, _position.y, 0.0f);
		glRotatef(_position.angle, 0.0f, 0.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);

		if (_isKilled)
		{
			_DrawPlayerBlowUp();
			glPopMatrix();
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		else
		{
			// Draw engine fire right after the ship.
			if (_thrust)
			{
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				glVertexPointer(2, GL_FLOAT, 0, _thrustVectors);
				glDrawArrays(GL_LINE_STRIP, 0, 3);
			}

			// Draw ship itself.
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			glVertexPointer(2, GL_FLOAT, 0, _vertices);
			glDrawArrays(GL_LINE_STRIP, 0, 5);

			glPopMatrix();

			// Draw any other parts.
			if (_machineGun != 0)
			{
				_machineGun->Draw();
			}

			glDisableClientState(GL_VERTEX_ARRAY);
		}
	}

	// Updates coordinates, angles, etc.
	void Player::Update(float time, float total)
	{
		if (_isKilled)
		{
			return;
		}

		if (_left)
		{
			_position.angle = _position.angle + time * 120;
		}
		if (_right)
		{
			_position.angle = _position.angle - time * 120;
		}
		if (_thrust)
		{
			_position.xv = _position.xv + cos(_position.angle*MATH_PI/180.0) * time * 40;
			_position.yv = _position.yv + sin(_position.angle*MATH_PI/180.0) * time * 40;
		}

		float x = _position.x + _position.xv * time;
		x = x / ORTHO_MAX_D_X;
		x = (x - floor(x))*ORTHO_MAX_D_X;

		if (x < 1 || x > ORTHO_MAX_D_X - 1)
		{
			_position.xv = -_position.xv;  // < ---- MODE 2. Remove this lines and ship will not stop on screen boundaries.

			x = _position.x + _position.xv * time;
			x = x / ORTHO_MAX_D_X;
			x = (x - floor(x))*ORTHO_MAX_D_X;

			_position.xv = 0;  // < ---- MODE 1. Remove this and ship will bump up like a crazy =).
		}

		float y = _position.y + _position.yv * time;
		y = y / ORTHO_MAX_D_Y;
		y = (y - floor(y))*ORTHO_MAX_D_Y;

		if (y < 1 || y > ORTHO_MAX_D_Y - 1)
		{
			_position.yv = -_position.yv;  // < ---- MODE 2.

			y = _position.y + _position.yv * time;
			y = y / ORTHO_MAX_D_Y;
			y = (y - floor(y))*ORTHO_MAX_D_Y;

			_position.yv = 0;  // < ---- MODE 1.
		}

		_position.x = x;
		_position.y = y;

		// Update other parts
		if (_machineGun != 0)
		{
			_machineGun->Update(time, total);
		}
	}

	// Handles user inputs to manipulate player.
	void Player::Touch(int id, float time, float x, float y)
	{
		float actualY = ORTHO_MAX_D_Y - y;

		if (x < INPUT_BUTTON_SIZE && actualY < INPUT_BUTTON_SIZE)
		{
			_thrust = 1;
		}
		else if (x > ORTHO_MAX_D_X - INPUT_BUTTON_SIZE && actualY < INPUT_BUTTON_SIZE)
		{
			_MakeShot(time);
		}
		else if (x < INPUT_BUTTON_SIZE && actualY > INPUT_BUTTON_SIZE)
		{
			_left = 1;
		}
		else if (x > ORTHO_MAX_D_X - INPUT_BUTTON_SIZE && actualY > INPUT_BUTTON_SIZE)
		{
			_right = 1;
		}
	}

	// Handles user inputs to manipulate player.
	void Player::Untouch(int id, float time, float x, float y)
	{
		float actualY = ORTHO_MAX_D_Y - y;

		if (x < INPUT_BUTTON_SIZE && actualY < INPUT_BUTTON_SIZE)
		{
			_thrust = 0;
		}
		else if (x < INPUT_BUTTON_SIZE && actualY > INPUT_BUTTON_SIZE)
		{
			_left = 0;
		}
		else if (x > ORTHO_MAX_D_X - INPUT_BUTTON_SIZE && actualY > INPUT_BUTTON_SIZE)
		{
			_right = 0;
		}
	}

	// Draws blow up when player was killed.
	void Player::_DrawPlayerBlowUp()
	{
		// Draw simple animation.
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

		for (int i = 0; i < 8; i+=2)
		{
			GLfloat dx = _killedDelta;
			GLfloat dy = _killedDelta;

			if (_vertices[i] > _vertices[i+2])
			{
				dx = -_killedDelta;
			}
			else
			{
				dx = _killedDelta;
			}

			if (_vertices[i + 1] > _vertices[i+3])
			{
				dy = -_killedDelta;
			}
			else
			{
				dy = _killedDelta;
			}

			GLfloat* temp = new GLfloat[4];

			temp[0] = _vertices[i] + dx;
			temp[1] = _vertices[i + 1] + dy;
			temp[2] = _vertices[i + 2] + dx;
			temp[3] = _vertices[i + 3] + dy;

			glVertexPointer(2, GL_FLOAT, 0, temp);
			glDrawArrays(GL_LINE_STRIP, 0, 2);
		}

		_killedDelta += 1.0;
	}

	// Gives a command to make shot for all weapons.
	void Player::_MakeShot(float time)
	{
		if (_machineGun != 0)
		{
			_machineGun->MakeShot(time, _position);
		}

	}

	// TODO Return read only smart pointer
	Position* Player::GetPosition()
	{
		return &_position;
	}

	// Detect if player hit some enemy by its weapon.
	bool Player::IsHit(AsteroidEnemy* enemy)
	{
		if (_machineGun != 0)
		{
			return _machineGun->IsDamagedByBullets(enemy);
		}

		return false;
	}

	// Detect if ship is damaged by enemy.
	bool Player::IsDamagedByEnemy(AsteroidEnemy* enemy)
	{
		float enemyX = enemy->GetX();
		float enemyY = enemy->GetY();

		// Find intersection
		float r = sqrt(pow(abs(_position.x - enemyX), 2) + pow(abs(_position.y - enemyY), 2));

		if (r <= enemy->GetOuterRadius() + 1.0)
		{
			return true;
		}

		return false;
	}

	void Player::SetKilled(bool isKilled)
	{
		_isKilled = isKilled;
	}

	bool Player::IsKilled()
	{
		return _isKilled;
	}
}
