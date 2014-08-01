#pragma once
#ifndef __machinegun_h__
#define __machinegun_h__

#include <vector>
#include "Bullet.h"
#include "GraphicObject.h"
#include "Position.h"
#include "AsteroidEnemy.h"

namespace asteroids
{
	// Describes behavior of a user weapon.
	class MachineGun : public GraphicObject
	{
	public:
		MachineGun();
		virtual void Draw();
		virtual void Update(float time, float total);

		// Creates new bullets.
		void MakeShot(float time, Position playerPosition);

		// Calculates if some bullets damaged objects.
		bool IsDamagedByBullets(AsteroidEnemy* enemy);

	private:
		// Which free bullet to select as new.
		int _GetBulletToShow(void);
		// Initialize new bullet.
		void _InitBullet(int i, float time, Position playerPosition);

		// Bullets pre-cached.
		std::vector<Bullet> _bullets;
	};
}

#endif /* __machinegun_h__ */

