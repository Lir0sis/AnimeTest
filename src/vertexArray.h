#pragma once

#include "../main.h"
#include "layout.h"

namespace CourseLab 
{
	class VertexArray 
	{
	private:
		GLuint m_arrayID;
		VertexLayout m_layout;
	public:
		VertexArray();
		~VertexArray();

		GLuint GetID();
		void AddLayout(VertexLayout layout) { m_layout = layout; }
		void ActivateLayout() const;
		void Bind() const;
		void Unbind() const;
	};
}