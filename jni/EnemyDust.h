#pragma once
#ifndef __enemydust_h__
#define __enemydust_h__

#include "GraphicObject.h"
#include "BaseEnemy.h"

namespace asteroids
{
	// Describes behavior of the enemy Dust: parts of the enemy which cannot be destroyed.
	class EnemyDust : public BaseEnemy, public GraphicObject
	{
	public:
		EnemyDust();
		virtual ~EnemyDust();
		// Main method for drawing shape.
		virtual void Draw();
		// Updates coordinates of the enemy.
		virtual void Update(int time, int total = 0);
		// Returns outside radius of the shape.
		virtual float GetOuterRadius();

	private:
		float _deltaX;
		float _deltaY;

		// Pre-calculated angle values for faster X and Y calculations.
		float _cosA;
		float _sinA;
	};
}

#endif /* __enemydust_h__ */
