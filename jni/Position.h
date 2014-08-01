#pragma once
#ifndef __position_h__
#define __position_h__

namespace asteroids
{
	// Keeps position of game element.
	struct Position
	{
		Position(): x(0), y(0), xv(0), yv(0), angle(0) {};

		void SetPosition(float xx, float yy)
		{
			x = xx;
			y = yy;
		}

		void Translate(float xx, float yy)
		{
			x += xx;
			y += yy;
		}

		float x;
		float y;
		float xv;
		float yv;

		float angle;
	};
}

#endif /* __position_h__ */
