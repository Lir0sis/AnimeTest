#pragma once

#include <../main.h>

namespace CourseLab 
{
	void resizeCallback(GLFWwindow* window, int width, int height)
	{
		std::cout << "called callback" << std::endl;
		glViewport(0, 0, width, height);
	}
}
