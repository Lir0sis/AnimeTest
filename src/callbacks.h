#pragma once

#include <../main.h>

namespace CourseLab
{
	void resizeCallback(GLFWwindow* window, int width, int height)
	{
		std::cout << "called callback" << std::endl;
		glViewport(0, 0, width, height);
	}

	inline void gl_callback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam
	) {

		/*std::printf(
			"OpenGL error: t{ 0x%x }, s{ 0x%x }, lvl{ 0x%x }\n\tmsg: %s \n",
			type, source, severity, message
		);*/

	}
}
