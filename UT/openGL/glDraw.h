#pragma once

#pragma comment (lib, "OpenGL32.lib")
#include <gl/GL.h>

namespace GL
{
	void SetupOrtho();

	void DrawLine(float x1, float y1, float x2, float y2, const GLubyte color[3]);
}

namespace color
{
	const GLubyte red[3] = { 255, 0, 0 };
}