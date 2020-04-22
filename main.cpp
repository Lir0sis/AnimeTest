
#include "src/window.h"
#include "src/shader.h"
#include "src/vertexArray.h"
#include "src/objects.h"

#include <imGui/imgui.h>
#include <imGui/imgui_impl_opengl3.h>
#include <imGui/imgui_impl_glfw.h>
#define PI_CONST 3.1415926f

GLint wWidth = 640, wHeight = 420;

const char *glsl_version = "#version 130";

#define BATCH_BUFFER_SIZE 0; // The last thing to do

void processInput(GLFWwindow* window, CourseLab::Camera& cam, float time);

int main(char* arv[], int argc) 
{
	auto window = std::make_shared<CourseLab::AppWindow>(wWidth, wHeight);

	CourseLab::ShaderProgram program("res/shader");
	CourseLab::Model model(new CourseLab::File("sdsds"), "C:/Users/Valentine/Desktop/narancha/0.obj");

	model.Translate(glm::vec3(0.0f, -0.5f, 0.0f));
	//model.Rotate(glm::vec3(-(90.0f / 180.0f) * PI_CONST, 0.0f, 0.0f));

	CourseLab::Camera cam(glm::vec3(0.0f, 0.25f, 3.0f));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window->glWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	bool show_demo_window = true;
	float i = 0;

	while (!window->shouldClose())
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow(&show_demo_window);

		i += 0.01f;
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		glfwGetFramebufferSize(window->glWindow, &wWidth, &wHeight);
		glm::mat4 proj = glm::perspective(45.0f, (float)wWidth / (float)wHeight, 0.1f, 100.0f);

		program.Bind();

		// Light

		GLCall(glUniform3fv(glGetUniformLocation(program.ID(), "light.position"), 1, glm::value_ptr(glm::vec3(2.0f * cosf(i * 2), 4.0f, 2.0f * sinf(i * 2)))));
		GLCall(glUniform3fv(glGetUniformLocation(program.ID(), "light.viewPos"), 1, glm::value_ptr(cam.Pos())));
		//glm::vec3 lightColor(1.0f * cosf(i), 0.5f, 1.0f * sinf(i));
		glm::vec3 lightColor(1.0f);
		GLCall(glUniform3fv(glGetUniformLocation(program.ID(), "light.ambient"), 1, glm::value_ptr(lightColor * 0.5f)));
		GLCall(glUniform3fv(glGetUniformLocation(program.ID(), "light.diffuse"), 1, glm::value_ptr(lightColor * 0.2f)));
		GLCall(glUniform3fv(glGetUniformLocation(program.ID(), "light.specular"), 1, glm::value_ptr(lightColor)));

		//
		cam.Pos(glm::vec3(2.0f * cosf(i / 2), 1.0f, 2.0f * sinf(i / 2)));
		//model.Size(glm::vec3(1.0f * cosf(i), 0.5f, 1.0f * sinf(i)));
		model.Draw(program.ID(), proj * cam.GetViewMatrix());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window->glWindow);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	std::cout << "fisting is 300$";

}

void processInput(GLFWwindow* window, CourseLab::Camera& cam, float time)
{
	float cameraSpeed = 0.5f * time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.Translate(cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.Translate(cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.Translate(glm::vec3(-1.0f, 0.0f, 0.0f) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.Translate(glm::vec3(1.0f, 0.0f, 0.0f) * cameraSpeed);
}
