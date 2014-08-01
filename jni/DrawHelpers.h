#ifndef DRAWHELPERS_H_
#define DRAWHELPERS_H_

#include <GLES/gl.h>
#include <EGL/egl.h>

namespace asteroids
{
	// Contains methods for drawing additional graphics.
	class DrawHelpers
	{
	public:
		static void DrawMovingButton();
		static void DrawRightButton();
		static void DrawLeftButton();
		static void DrawGunButton();

		static void DrawGameName();
		static void DrawRestartQuestion();

	private:
		static void _GLDrawEllipse(int segments, GLfloat width, GLfloat height, GLfloat centerX, GLfloat centerY, bool filled);

	private:
		const static GLfloat _header[];
		const static GLfloat _restart[];
		const static GLfloat _exit[];
		const static GLfloat _gunLabel[];
		const static GLfloat _leftLabel[];
		const static GLfloat _rightLabel[];
		const static GLfloat _moveLabel[];
	};

} /* namespace Asteroids */
#endif /* DRAWHELPERS_H_ */
