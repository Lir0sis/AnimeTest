#pragma once

#include "../main.h"
#include "layout.h"
#include "buffer.h"

namespace CourseLab 
{
	class VertexArray 
	{
	private:
		bool layoutInited;
		GLuint m_arrayID;
		VertexLayout m_layout;
	public:
		VertexArray();
		~VertexArray();

		const VertexBuffer* vbo;
		const IndexBuffer* ibo;
		GLuint GetID();

		inline void BindBuffers(const IndexBuffer* indecies, const VertexBuffer* vertices) { vbo = vertices; ibo = indecies; }
		inline void AddLayout(const VertexLayout& layout) { layoutInited = false; m_layout = layout; }
		void ActivateLayout();
		void Bind() const;
		void Unbind() const;

		VertexArray(const VertexArray&) = delete;
		const VertexArray& operator=(const VertexArray&) = delete;
	};
}