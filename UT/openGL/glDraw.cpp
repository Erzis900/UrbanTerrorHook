#include "pch.h"
#include "glDraw.h"

namespace GL
{
	void SetupOrtho()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);

		glOrtho(0, m_viewport[2], m_viewport[3], 0, 1, -1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void DrawLine(float x1, float y1, float x2, float y2, const GLubyte color[3])
	{
		glColor3ub(color[0], color[1], color[2]);
		glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
	}
}