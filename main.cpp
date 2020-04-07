
#include "main.h"
#include "src/window.h"
#include "src/shader.h"
#include "src/buffer.h"
#include "src/vertexArray.h"

GLfloat data[] = {
	-0.5f, -0.5f, 0.1f, 0.3f, 0.0f,
	 0.0f,  0.5f, 1.0f, 0.3f, 0.9f,
	 0.5f, -0.5f, 0.0f, 0.0f, 0.3f
};

GLuint indeces[] = { 0, 1, 2 };

int main(char* arv[], int argc) 
{
	
	std::shared_ptr<CourseLab::AppWindow> window{ new CourseLab::AppWindow(640, 420) };
	CourseLab::ShaderProgram program("D:\\Valik\\dev\\GitHub\\CourseLab\\res\\shader");

	CourseLab::VertexArray vao;
	CourseLab::IndexBuffer ib(sizeof(indeces), indeces);
	CourseLab::VertexBuffer vb(sizeof(data), data);

	CourseLab::VertexLayout layout;
	layout.PushAttrib({ 2, GL_FLOAT, false }); //pos
	layout.PushAttrib({ 3, GL_FLOAT, false }); //color

	vao.AddLayout(layout);
	vao.ActivateLayout();

	vb.Unbind();
	ib.Unbind();
	vao.Unbind();
	
	//glDisableVertexArrayAttrib(vao.GetID(), 1);

	while (!window->shouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		program.Bind();
		vao.Bind();
		ib.Bind();

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window->glWindow);
		glfwPollEvents();
	}

	std::cout << "fuck you";

}