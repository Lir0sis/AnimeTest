#pragma once

#include "../main.h"

namespace CourseLab 
{
	struct ShaderSource 
	{
		std::string vertexSrc;
		std::string fragmentSrc;
	};

	class ShaderProgram 
	{
	private:
		std::string path;
		GLuint m_ProgramID;

	public:
		ShaderProgram(const char* dirpath);
		~ShaderProgram();
		
		GLuint CompileShader(GLuint type, const std::string& src);
		ShaderSource ReadShader(const std::string& dirpath);
		void CreateShaders(std::string& dirpath);

		void Bind() const;
		void Unbind() const;
	};

}