
#include "window.h"
#include "callbacks.h"

namespace CourseLab 
{

	AppWindow::AppWindow(GLint width, GLint height) : width(width), height(height), glWindow(nullptr) 
	{
		if (!glfwInit())
			throw std::runtime_error("Couldn\'t init Opengl");
			
		GLCall(AppWindow::glWindow = glfwCreateWindow(width, height, "Hello World", NULL, NULL));
		if (!glWindow)
			throw std::runtime_error("Failed to create window");

		GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3));
		GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3));
		GLCall(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));

		GLCall(glfwWindowHint(GLFW_RESIZABLE, GL_FALSE));

		GLCall(glfwSetFramebufferSizeCallback(glWindow, resizeCallback));

		GLCall(glfwMakeContextCurrent(glWindow));

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to initialize OpenGL context");

		GLCall(glViewport(0, 0, width, height));
	}

	AppWindow::~AppWindow() 
	{
		GLCall(glfwTerminate());
	}
	
	int AppWindow::shouldClose() const
	{
		return glfwWindowShouldClose(glWindow);
	}

}