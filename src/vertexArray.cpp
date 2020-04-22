#include "vertexArray.h"
namespace CourseLab
{
	VertexArray::VertexArray() : vbo(nullptr), ibo(nullptr), layoutInited(false)
	{
		GLCall(glGenVertexArrays(1, &m_arrayID));
		GLCall(glBindVertexArray(m_arrayID));
	}

	VertexArray::~VertexArray() 
	{
		GLCall(glDeleteVertexArrays(1, &m_arrayID));
	}
	void VertexArray::ActivateLayout()
	{
		if (!layoutInited) {
			std::vector<LayoutAttrib> elements = m_layout.GetElements();
			int stride = m_layout.GetStride();

			Bind();
			int offset = 0;

			for (int i = 0; i < elements.size(); i++)
			{
				GLCall(glEnableVertexAttribArray(i));
				GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type,
					elements[i].normalized ? GL_TRUE : GL_FALSE,
					stride * sizeof(GLfloat), (const void*)(offset * sizeof(GLfloat))));

				offset += elements[i].count;
			}
		layoutInited = true;
		}
		else 
		{
			for (int i = 0; i < m_layout.GetSize(); i++) 
			{
				GLCall(glEnableVertexAttribArray(i));
			}
		}
	}
	void VertexArray::Unbind() const 
	{
		GLCall(glBindVertexArray(0));
	}
	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_arrayID));
	}

	GLuint VertexArray::GetID() { return m_arrayID; }
}
