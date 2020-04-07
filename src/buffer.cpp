
#include "buffer.h"

namespace CourseLab
{
	VertexBuffer::VertexBuffer(GLuint size, const void* data)
	{
		glGenBuffers(1, &m_vertexID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_vertexID);
	}

	void VertexBuffer::Bind() const 
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	///////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(GLuint count, const GLuint* data) : m_count(count)
	{
		glGenBuffers(1, &m_indexID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
	}
	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &m_indexID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}