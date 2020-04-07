
#include "window.h"

namespace CourseLab 
{

	AppWindow::AppWindow(GLint width, GLint height) : width(width), height(height), glWindow(nullptr) 
	{
		if (!glfwInit())
			throw std::runtime_error("we've fucked up #1");
			
		AppWindow::glWindow = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
		if (!glWindow)
			throw std::runtime_error("we've fucked up #2");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		glfwMakeContextCurrent(glWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to initialize OpenGL context");

		glViewport(0, 0, width, height);
	}

	AppWindow::~AppWindow() 
	{
		glfwTerminate();
	}
	
	int AppWindow::shouldClose() const
	{
		return glfwWindowShouldClose(glWindow);
	}

}