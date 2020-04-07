#include "vertexArray.h"
namespace CourseLab
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_arrayID);
		glBindVertexArray(m_arrayID);
	}

	VertexArray::~VertexArray() 
	{
		glDeleteVertexArrays(1, &m_arrayID);
	}
	void VertexArray::ActivateLayout() const
	{
		std::vector<LayoutAttrib> elements = m_layout.GetElements();
		int stride = m_layout.GetStride();

		Bind();
		int offset = 0;

		for (int i = 0; i < elements.size(); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized ? GL_TRUE : GL_FALSE, stride * sizeof(GLfloat), (const void*)(offset * sizeof(GLfloat)));
			offset += elements[i].count;
		}
	}
	void VertexArray::Unbind() const 
	{
		glBindVertexArray(0);
	}
	void VertexArray::Bind() const
	{
		glBindVertexArray(m_arrayID);
	}

	GLuint VertexArray::GetID() { return m_arrayID; }
}
