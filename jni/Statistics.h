#pragma once
#ifndef __statistics_h__
#define __statistics_h__

#include <vector>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>

namespace asteroids
{
	// Responsible for contain, keep and draw game statistics.
	class Statistics
	{
	public:
		Statistics(int32_t screenWidth, int32_t screenHeight);
		virtual ~Statistics();

		// Manipulate player lives.
		void IncreaseLives();
		void DecreaseLives();
		int GetLivesNumber();

		// Manipulate current level.
		void IncreaseLevel();

		// Manipulate points.
		void IncreasePointsEarned();

		void Draw();

	private:
		// Split number to digits for drawing on display.
		void _SplitToDigits(int number, std::vector<int>* array);

		// Draws one digit in place of X, Y.
		void _DrawDigit(int digit, GLfloat placeX, GLfloat placeY);

	private:
		std::vector<int> _levels;
		std::vector<int> _points;

		int _currentLives;
		int _currentLevels;
		int _currentPoints;

		int32_t _screenWidth;
		int32_t _screenHeight;

		const static GLfloat _ship[];
		const static GLfloat _zero[];
		const static GLfloat _one[];
		const static GLfloat _two[];
		const static GLfloat _three[];
		const static GLfloat _four[];
		const static GLfloat _five[];
		const static GLfloat _six[];
		const static GLfloat _seven[];
		const static GLfloat _eight[];
		const static GLfloat _nine[];
	};
}

#endif /* __statistics_h__ */

