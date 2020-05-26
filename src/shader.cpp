
#include "shader.h"
namespace CourseLab
{
	ShaderProgram::ShaderProgram(const char* dirpath) : m_ProgramID(0)
	{
		path = dirpath;
		CreateShaders(path);
	}
	ShaderProgram::~ShaderProgram() 
	{
		GLCall(glDeleteProgram(m_ProgramID));
	}

	void ShaderProgram::CreateShaders(std::string& dirpath)
	{
		if (!m_ProgramID) { 
			GLCall(m_ProgramID = glCreateProgram();)
				LOG("Created Shader Program")
		}

		ShaderSource ss = ReadShader(path);
		GLuint vertex = CompileShader(GL_VERTEX_SHADER, ss.vertexSrc),
			fragment = CompileShader(GL_FRAGMENT_SHADER, ss.fragmentSrc);

		LOG("Compiled Shaders")

		GLCall(glAttachShader(m_ProgramID, vertex));
		GLCall(glAttachShader(m_ProgramID, fragment));
		GLCall(glLinkProgram(m_ProgramID));

		GLint success;
		GLCall(glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLchar infoLog[512];
			glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		LOG("Linked Shaders")
		GLCall(glDeleteShader(vertex));
		GLCall(glDeleteShader(fragment));

	}

	GLuint ShaderProgram::CompileShader(GLuint type, const std::string& src)
	{
		GLCall(GLuint shaderID = glCreateShader(type));
		const GLchar* source = src.c_str();
		GLCall(glShaderSource(shaderID, 1, &source, nullptr));
		GLCall(glCompileShader(shaderID));

		GLint status; GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));

		if (status != GL_TRUE) {

			GLchar infoLog[512];
			GLCall(glGetShaderInfoLog(shaderID, 512, NULL, infoLog));
			std::cout << infoLog;

			//throw std::runtime_error("We've fucked up...");
		}
		
		return shaderID;
	}

	void ShaderProgram::Bind() const
	{
		GLCall(glUseProgram(m_ProgramID));
	}

	void ShaderProgram::Unbind() const 
	{
		glUseProgram(0);
	}

	ShaderSource ShaderProgram::ReadShader(const std::string& dirpath) 
	{
		std::ifstream vertexF(path + "\\vertex.shader");
		std::ifstream fragmentF(path + "\\fragment.shader");

		std::stringstream vstream, fstream;
		vstream << vertexF.rdbuf();
		fstream << fragmentF.rdbuf();
		
		vertexF.close(); fragmentF.close();

		return { vstream.str(), fstream.str().c_str() };
	}

}