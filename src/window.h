#pragma once

#include "../main.h"

namespace CourseLab 
{

	class AppWindow 
	{ 

	private:
		GLint width, height;

	public:
		AppWindow(GLint width, GLint height);
		~AppWindow();

		GLFWwindow* glWindow;
		
		int shouldClose() const;
	};

}