
#include "window.h"
#include "callbacks.h"

namespace CourseLab 
{
	AppWindow::AppWindow(GLint width, GLint height) : width(width), height(height), glWindow(nullptr) 
	{
		if (!glfwInit())
			throw std::runtime_error("Couldn\'t init Opengl");


		AppWindow::glWindow = glfwCreateWindow(width, height, "CourseLab", NULL, NULL);
		if (!glWindow)
			throw std::runtime_error("Failed to create window");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

		glfwSetFramebufferSizeCallback(glWindow, resizeCallback);

		glfwMakeContextCurrent(glWindow);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw std::runtime_error("Failed to initialize OpenGL context");

		GLCall(glViewport(0, 0, width, height));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_DEBUG_OUTPUT));
		GLCall(glDebugMessageCallback(gl_callback, 0));
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