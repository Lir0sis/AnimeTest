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
				size_t value;

				switch (elements[i].type) {
				case GL_INT:
					value = sizeof(GLint);
					break;
				case GL_FLOAT:
					value = sizeof(GLfloat);
					break;
				case GL_UNSIGNED_INT:
					value = sizeof(GLuint);
					break;
				}


				GLCall(glEnableVertexAttribArray(i));
				if (elements[i].type == GL_INT)
				{
					GLCall(glVertexAttribIPointer(i, elements[i].count, elements[i].type,
						stride * value, (const void*)(offset)));
				}
				else
				{
					GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type,
						elements[i].normalized ? GL_TRUE : GL_FALSE,
						stride * value, (const void*)(offset)));
				}

				offset += elements[i].count * value;
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
