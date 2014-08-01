#pragma once
#ifndef __baseenemy_h__
#define __baseenemy_h__

#include <EGL/egl.h>

namespace asteroids
{
	// Base enemy object with necessary common functions and fields.
	class BaseEnemy
	{
	public:
		BaseEnemy();
		virtual ~BaseEnemy() {};

		float GetX();
		float GetY();

		// Returns outside radius.
		virtual float GetOuterRadius() = 0;

	protected:
		// Returns initial far distance from the point where shape is coming to.
		float _GetInitialDistance(float angle, float goalX, float goalY, float width, float height);
		// Returns randomly generated angle to the center of view.
		float _GetRandomAngle();
		// Returns randomly generated type of movement. This is determines
		// which point of the screen is the point where shape is coming to.
		int _GetRandomTypeOfMovement();
		// Returns any random number.
		float _GetRandomNumber(int max);

	protected:
		// Angle to the center of window.
		float _angle;
		// Distance to the cener of window.
		float _distance;
		// Coordinates.
		float _x;
		float _y;
		// Movement type.
		int _movementType;
	};
}

#endif /* __baseenemy_h__ */
