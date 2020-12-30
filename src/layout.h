#pragma once

#include "main.h"

namespace CourseLab 
{
	struct LayoutAttrib {
		int count;
		GLuint type;
		bool normalized;
	};

	class VertexLayout 
	{
	private:
		int m_stride;
		std::vector<LayoutAttrib> m_elements;

	public:
		VertexLayout() : m_stride(0) {}
		~VertexLayout() {}

		void PushAttrib(LayoutAttrib attrib) 
		{
			m_elements.push_back(attrib);
			m_stride += attrib.count;
		}

		void SetDefault() {
			PushAttrib({ 3, GL_FLOAT, false });
			PushAttrib({ 2, GL_FLOAT, false });
			PushAttrib({ 3, GL_FLOAT, false }); 
		}

		inline GLuint GetSize() { return (GLuint)m_elements.size(); }

		inline const std::vector<LayoutAttrib>& GetElements() const { return m_elements; }
		inline int GetStride() const { return m_stride; }
	};
}