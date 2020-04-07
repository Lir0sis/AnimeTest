#pragma once

#include "../main.h"

namespace CourseLab 
{
	class VertexBuffer 
	{
	private:
		GLuint m_vertexID;
	public:
		VertexBuffer(GLuint size, const void* data);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	};

	class IndexBuffer
	{
	private:
		GLuint m_indexID;
		GLuint m_count;
	public:
		IndexBuffer(GLuint count, const GLuint* data);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;
	};

}