#include "DrawHelpers.h"
#include "Common.h"

#include <GLES/glext.h>

namespace asteroids
{
	// Arrays of vectors describes words and letters. Simpliest implementation
	// instead of using FreeType lib.
	const GLfloat DrawHelpers::_header[] = {
		// A
		-57.0, 0.0,
		-65.0, -5.0,
		-60.0, 5.0,
		-55.0, -5.0,
		// S
		-50.0, -5.0,
		-40.0, -2.0,
		-50.0, 2.0,
		-40.0, 5.0,
		// T
		-35.0, 5.0,
		-25.0, 5.0,
		-30.0, 5.0,
		-30.0, -5.0,
		// E
		-10.0, 5.0,
		-20.0, 5.0,
		-20.0, -5.0,
		-10.0, -5.0,
		// R
		-5.0, -5.0,
		-5.0, 5.0,
		 5.0, 3.0,
		-5.0, -1.0,
		 5.0, -5.0,
		 // O
		 10.0, -5.0,
		 10.0, 5.0,
		 20.0, 5.0,
		 20.0, -5.0,
		 10.0, -5.0,
		 // I
		 32.0, -5.0,
		 30.0, -5.0,
		 30.0, 5.0,
		 28.0, 5.0,
		 32.0, 5.0,
		 30.0, 5.0,
		 30.0, -5.0,
		 // D
		 40.0, -5.0,
		 40.0, 5.0,
		 50.0, 0.0,
		 40.0, -5.0,
		 // S
		 55.0, -5.0,
		 65.0, -2.0,
		 55.0, 2.0,
		 65.0, 5.0
	};
	const GLfloat DrawHelpers::_restart[] = {
		// R
		-100.0, -5.0,
		-100.0, 5.0,
		-90.0, 5.0,
		-90.0, 0.0,
		-100.0, 0.0,
		-90.0, -5.0,
		// E
		-85.0, 0.0,
		-75.0, 5.0,
		-85.0, 5.0,
		-85.0, -5.0,
		// S
		-70.0, -5.0,
		-60.0, -2.0,
		-70.0, 2.0,
		-60.0, 5.0,
		// T
		-55.0, 5.0,
		-45.0, 5.0,
		-50.0, 5.0,
		-50.0, -5.0,
		// A
		-37.0, 0.0,
		-45.0, -5.0,
		-40.0, 5.0,
		-35.0, -5.0,
		// R
		-30.0, -5.0,
		-30.0, 5.0,
		-25.0, 5.0,
		-25.0, 0.0,
		-30.0, 0.0,
		-25.0, -5.0,
		// T
		-20.0, 5.0,
		-10.0, 5.0,
		-15.0, 5.0,
		-15.0, -5.0
	};
	const GLfloat DrawHelpers::_exit[] = {
		// E
		35.0, 5.0,
		25.0, 5.0,
		25.0, 0.0,
		35.0, 0.0,
		25.0, 0.0,
		25.0, -5.0,
		35.0, -5.0,
		// X
		40.0, -5.0,
		50.0, 5.0,
		45.0, 0.0,
		40.0, 5.0,
		50.0, -5.0,
		// I
		60.0, -5.0,
		60.0, 5.0,
		58.0, 5.0,
		62.0, 5.0,
		60.0, 5.0,
		60.0, -5.0,
		// T
		65.0, 5.0,
		75.0, 5.0,
		70.0, 5.0,
		70.0, -5.0
	};
	const GLfloat DrawHelpers::_gunLabel[] = {
		-10.0, 0.0,
		0.0, 0.0,
		0.0, 10.0,
		0.0, 0.0,
		10.0, 0.0,
		0.0, 0.0,
		0.0, -10.0,
		0.0, 0.0
	};
	const GLfloat DrawHelpers::_leftLabel[] = {
		10.0, 10.0,
		-10.0, 0.0,
		10.0, -10.0
	};
	const GLfloat DrawHelpers::_rightLabel[] = {
		-10.0, 10.0,
		10.0, 0.0,
		-10.0, -10.0
	};
	const GLfloat DrawHelpers::_moveLabel[] = {
		-10.0, -10.0,
		0.0, 10.0,
		10.0, -10.0
	};

	void DrawHelpers::DrawMovingButton()
	{
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		_GLDrawEllipse(12, 20.0, 20.0, 10.0, 10.0, false);

		// Draw label
		glVertexPointer(2, GL_FLOAT , 0, _moveLabel);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	}

	void DrawHelpers::DrawRightButton()
	{
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		_GLDrawEllipse(12, 20.0, 20.0, ORTHO_MAX_D_X - 10.0, ORTHO_MAX_D_Y - 10.0, false);

		// Draw label
		glVertexPointer(2, GL_FLOAT , 0, _rightLabel);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	}

	void DrawHelpers::DrawLeftButton()
	{
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		_GLDrawEllipse(12, 20.0, 20.0, 10.0, ORTHO_MAX_D_Y - 10.0, false);

		// Draw label
		glVertexPointer(2, GL_FLOAT , 0, _leftLabel);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	}

	void DrawHelpers::DrawGunButton()
	{
		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		_GLDrawEllipse(12, 20.0, 20.0, ORTHO_MAX_D_X - 10.0, 10.0, false);

		// Draw label
		glVertexPointer(2, GL_FLOAT , 0, _gunLabel);
		glDrawArrays(GL_LINE_LOOP, 0, 8);

		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	}

	void DrawHelpers::DrawGameName()
	{
		glPushMatrix();
		glTranslatef(ORTHO_MAX_D_X / 2.0, ORTHO_MAX_D_Y / 2.0, 0.0);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, _header);
		glDrawArrays(GL_LINE_STRIP, 0, 41);
		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	}

	void DrawHelpers::DrawRestartQuestion()
	{
		glPushMatrix();
		glTranslatef(ORTHO_MAX_D_X / 2.0, ORTHO_MAX_D_Y / 2.0, 0.0);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, _restart);
		glDrawArrays(GL_LINE_STRIP, 0, 32);

		glVertexPointer(2, GL_FLOAT, 0, _exit);
		glDrawArrays(GL_LINE_STRIP, 0, 22);

		glDisableClientState(GL_VERTEX_ARRAY);
		glPopMatrix();
	}

	void DrawHelpers::_GLDrawEllipse(int segments, GLfloat width, GLfloat height, GLfloat centerX, GLfloat centerY, bool filled)
	{
		glTranslatef(centerX, centerY, 0.0);
		GLfloat vertices[segments*2];
		int count=0;

		for (GLfloat i = 0; i < 360.0f; i+=(360.0f/segments))
		{
			vertices[count++] = (cos(i * MATH_PI / 180)*width);
			vertices[count++] = (sin(i * MATH_PI / 180)*height);
		}
		glVertexPointer (2, GL_FLOAT , 0, vertices);
		glDrawArrays ((filled) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, segments);
	}

} /* namespace Asteroids */
