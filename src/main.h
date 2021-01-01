#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <exception>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/matrix_interpolation.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define ASSERT(x) if (!(x)) __debugbreak();

#ifdef DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

#ifdef DEBUG
#define LOG(x) { std::cout << x << std::endl; }
#else
#define LOG(x)
#endif



static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* func, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") " << func << " in " << file << "on " << line << " line" << std::endl;
		return false;
	}
	return true;
}
