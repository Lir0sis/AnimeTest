
#include "src/window.h"
#include "src/shader.h"
#include "src/vertexArray.h"
#include "src/objects.h"

#include <assimp/Importer.hpp>

#include <imGui/imgui.h>
#include <imGui/imgui_impl_opengl3.h>
#include <imGui/imgui_impl_glfw.h>

#define PI_CONST 3.1415926f

GLint wWidth = 1024, wHeight = 768;

const char *glsl_version = "#version 330";

#define BATCH_BUFFER_SIZE 0; // The last thing to do

void processInput(GLFWwindow* window, CourseLab::Camera& cam, float time);

int main(char* arv[], int argc) 
{
	auto window = std::make_shared<CourseLab::AppWindow>(wWidth, wHeight);

	CourseLab::ShaderProgram program("res/shader");
	CourseLab::Model model("C:/Users/Valentine/Desktop/models/narancha/0.obj");
	CourseLab::Camera cam(glm::vec3(0.0f, 0.25f, 3.0f));
	CourseLab::LightSource light(0.7f, 0.2f, 1.0f, glm::vec3(1.0f));

	model.Translate(glm::vec3(0.0f, -0.75f, 0.0f));
	model.Size(glm::vec3(1.0f));

	light.Pos(glm::vec3(4.0f, 3.0f, 0.0f));

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
	glm::vec3 pos(0.0f);
	glm::vec3 rot(0.0f);
	glm::vec4 color(1.0f);
	while (!window->shouldClose())
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ColorEdit4("Color", glm::value_ptr(color));

		ImGui::SliderFloat("posX", &pos.x, -4.0f, 4.0f);
		ImGui::SliderFloat("posY", &pos.y, -4.0f, 4.0f);
		ImGui::SliderFloat("posZ", &pos.z, -4.0f, 4.0f);
		ImGui::SliderFloat("rotX", &rot.x, -4.0f, 4.0f);
		ImGui::SliderFloat("rotY", &rot.y, -4.0f, 4.0f);
		ImGui::SliderFloat("rotZ", &rot.z, -4.0f, 4.0f);

		//ImGui::ShowDemoWindow(&show_demo_window);

		i += 0.01f;
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Updating Data

		glfwGetFramebufferSize(window->glWindow, &wWidth, &wHeight);
		glm::mat4 proj = glm::perspective(45.0f, (float)wWidth / (float)wHeight, 0.1f, 100.0f);
		light.Pos(glm::vec3(2.0f * cosf(i * 2), 1.0f, 2.0f * sinf(i * 2)));
		cam.Pos(glm::vec3(2.0f * cosf(i / 2), 1.0f, 2.0f * sinf(i / 2)));
		light.SetColor(glm::vec3(color));
		model.Pos(pos);
		model.Rot(rot);

		/*glm::vec3(0.8f + 0.2f * cosf(i * 2), 0.9f + 0.1f * sin(i), 0.8f + 0.2f * sinf(i * 2))*/

		//Rendering

		program.Bind();
		light.SendData(program.ID(), cam.Pos());
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
