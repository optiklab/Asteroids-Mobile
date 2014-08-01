#pragma once
#ifndef __player_h__
#define __player_h__

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>

#include "MachineGun.h"
#include "GraphicObject.h"
#include "Position.h"
#include "AsteroidEnemy.h"

namespace asteroids
{
	// Describes behavior of the player.
	class Player : public GraphicObject
	{
	public:
		Player();

		virtual void Draw();
		virtual void Update(float time, float total);

		// Handles user inputs to manipulate player.
		void Touch(int id, float time, float x, float y);
		void Untouch(int id, float time, float x, float y);

		Position* GetPosition();

		// Detect if player hit some enemy by its weapon.
		bool IsHit(AsteroidEnemy* enemy);
		// Detect if ship is damaged by enemy.
		bool IsDamagedByEnemy(AsteroidEnemy* enemy);

		void SetKilled(bool isKilled);
		bool IsKilled();

	private:
		// Draws blow up when player was killed.
		void _DrawPlayerBlowUp();
		// Gives a command to make shot for all weapons.
		void _MakeShot(float time);

	private:
		// Contains temporary information when player was killed.
		GLfloat _killedDelta;
		bool _isKilled;

		// Buttons pressed and time of press.
		int _left;
		float _leftTime;
		int _right;
		float _rightTime;
		int _thrust;
		float _thrustTime;
		int _back;
		float _backTime;

		// Player position on display.
		Position _position;

		// Weapon.
		MachineGun *_machineGun;

		// Drawing elements for ship.
		const static GLfloat _vertices[];
		const static GLfloat _thrustVectors[];
	};
}
#endif /* __player_h__ */

