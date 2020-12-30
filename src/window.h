#pragma once

#include "main.h"

namespace CourseLab 
{

	class AppWindow 
	{ 
	private:
		GLint width, height;

	public:
		GLFWwindow* glWindow;

		AppWindow(GLint width, GLint height);
		~AppWindow();
		
		int shouldClose() const;

		void GetSize(GLint &width, GLint &height) {
			glfwGetFramebufferSize(glWindow, &this->width, &this->height);
			height = this->height;
			width = this->width;
		}
	};

}