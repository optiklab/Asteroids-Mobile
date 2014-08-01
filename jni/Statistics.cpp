#include "Statistics.h"
#include "Constants.h"
#include "Log.h"

namespace asteroids
{
	const GLfloat Statistics::_ship[] = {
			8.0, 0.0,
			-4.0, -4.0,
			-2.0, 0.0,
			-4.0, 4.0,
			8.0, 0.0
		};
	const GLfloat Statistics::_zero[] = {
			-5.0, -5.0,
			-5.0, 5.0,
			5.0, 5.0,
			5.0, -5.0,
			-5.0, -5.0
		};
	const GLfloat Statistics::_one[] = {
			-5.0, 0.0,
			5.0, 5.0,
			5.0, -5.0
		};
	const GLfloat Statistics::_two[] = {
			-5.0, 5.0,
			5.0, 5.0,
			5.0, 0.0,
			-5.0, -5.0,
			5.0, -5.0
		};
	const GLfloat Statistics::_three[] = {
			-5.0, 5.0,
			5.0, 5.0,
			5.0, 0.0,
			-5.0, 0.0,
			5.0, 0.0,
			5.0, -5.0,
			-5.0,-5.0
		};
	const GLfloat Statistics::_four[] = {
			5.0, 0.0,
			-5.0, 0.0,
			5.0, 5.0,
			5.0, -5.0
		};
	const GLfloat Statistics::_five[] = {
			5.0, 5.0,
			-5.0, 5.0,
			-5.0, 0.0,
			5.0, 0.0,
			5.0, -5.0,
			-5.0,-5.0
		};
	const GLfloat Statistics::_six[] = {
			5.0, 5.0,
			-5.0,5.0,
			-5.0,-5.0,
			5.0, -5.0,
			5.0, 0.0,
			-5.0,0.0
		};
	const GLfloat Statistics::_seven[] = {
			-5.0, 5.0,
			5.0, 5.0,
			5.0, -5.0
		};
	const GLfloat Statistics::_eight[] = {
			-5.0, 0.0,
			-5.0,5.0,
			5.0,5.0,
			5.0,0.0,
			-5.0,0.0,
			-5.0,-5.0,
			5.0,-5.0,
			5.0,0.0
		};
	const GLfloat Statistics::_nine[] = {
			-5.0, -5.0,
			5.0,-5.0,
			5.0,5.0,
			-5.0,5.0,
			-5.0,0.0,
			5.0,0.0
		};

	Statistics::Statistics(int32_t screenWidth, int32_t screenHeight)
	{
		_currentLives = DEFAULT_LIVES_COUNT;
		_currentLevels = 1;
		_currentPoints = 0;

		_levels.push_back(_currentLevels);
		_points.push_back(_currentPoints);

		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
	}

	Statistics::~Statistics()
	{
	}

	void Statistics::IncreaseLives()
	{
		++_currentLives;
	}

	void Statistics::DecreaseLives()
	{
		--_currentLives;
	}

	int Statistics::GetLivesNumber()
	{
		return _currentLives;
	}

	void Statistics::IncreaseLevel()
	{
		++_currentLevels;
		_SplitToDigits(_currentLevels, &_levels);
	}

	void Statistics::IncreasePointsEarned()
	{
		++_currentPoints;
		_SplitToDigits(_currentPoints, &_points);
	}

	// Draw statistics.
	void Statistics::Draw()
	{
		glEnableClientState(GL_VERTEX_ARRAY);

		// Draw points digit by digit.
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		int i = 0;
		for(int ii = _points.size() - 1; ii >= 0; ii--)
		{
			_DrawDigit(_points[ii], 95.0f + 15.0f * i++, ORTHO_MAX_D_Y - 10.0f);
		}

		// Draw levels digit by digit.
		for(int ii = _levels.size() - 1; ii >= 0; ii--)
		{
			_DrawDigit(_levels[ii], ORTHO_MAX_D_X - 40.0f, ORTHO_MAX_D_Y - 10.0f);
		}

		glDisableClientState(GL_VERTEX_ARRAY);

		// Draw lives number digit by digit.
		for(int j = 0; j < _currentLives; j++)
		{
			glPushMatrix();
			glTranslatef(j * 10.0f + 40.0f, ORTHO_MAX_D_Y - 10.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			glEnableClientState(GL_VERTEX_ARRAY);

			if (j == 0)
			{
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}

			glVertexPointer(2, GL_FLOAT, 0, _ship);
			glDrawArrays(GL_LINE_STRIP, 0, 5);
			glPopMatrix();
		}
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	// Show number on screen.
	void Statistics::_SplitToDigits(int number, std::vector<int>* array)
	{
		array->clear();

		// If number less than 10, we don't need to split.
		if (number < 10)
		{
			array->push_back(number);
			return;
		}

		int next = number;
		int toPrint = 0;

		// Split to digits in reverse order.
		while (next >= -1)
		{
			toPrint = next%10;
			next = next/10;

			// Another digit found.
			array->push_back(toPrint);

			if (next < 10)
			{
				// Last digit.
				array->push_back(next);
				break;
			}
		}
	}

	// Draw digit in screen place.
	void Statistics::_DrawDigit(int digit, GLfloat placeX, GLfloat placeY)
	{
		glPushMatrix();
		glTranslatef(placeX, placeY, 0.0); // TODO

		switch(digit)
		{
		case 0:
			glVertexPointer(2, GL_FLOAT, 0, _zero);
			glDrawArrays(GL_LINE_STRIP, 0, 5);
			break;
		case 1:
			glVertexPointer(2, GL_FLOAT, 0, _one);
			glDrawArrays(GL_LINE_STRIP, 0, 3);
			break;
		case 2:
			glVertexPointer(2, GL_FLOAT, 0, _two);
			glDrawArrays(GL_LINE_STRIP, 0, 5);
			break;
		case 3:
			glVertexPointer(2, GL_FLOAT, 0, _three);
			glDrawArrays(GL_LINE_STRIP, 0, 7);
			break;
		case 4:
			glVertexPointer(2, GL_FLOAT, 0, _four);
			glDrawArrays(GL_LINE_STRIP, 0, 4);
			break;
		case 5:
			glVertexPointer(2, GL_FLOAT, 0, _five);
			glDrawArrays(GL_LINE_STRIP, 0, 6);
			break;
		case 6:
			glVertexPointer(2, GL_FLOAT, 0, _six);
			glDrawArrays(GL_LINE_STRIP, 0, 6);
			break;
		case 7:
			glVertexPointer(2, GL_FLOAT, 0, _seven);
			glDrawArrays(GL_LINE_STRIP, 0, 3);
			break;
		case 8:
			glVertexPointer(2, GL_FLOAT, 0, _eight);
			glDrawArrays(GL_LINE_STRIP, 0, 8);
			break;
		case 9:
			glVertexPointer(2, GL_FLOAT, 0, _nine);
			glDrawArrays(GL_LINE_STRIP, 0, 6);
			break;
		default:
			break;
		}

		glPopMatrix();
	}
}
