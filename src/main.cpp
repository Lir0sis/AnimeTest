#include <../main.h>
#include "objects/renderer.h"

void processInput(GLFWwindow* window, CourseLab::Camera& cam, float time);

int main(char* arv[], int argc)
{
	CourseLab::Renderer* renderer = CourseLab::Renderer::Instance();

	while (!renderer->ShouldStop()) 
	{
		renderer->Update();
	}


	//float time = 0;

	//auto window = std::make_shared<CourseLab::AppWindow>(1024, 768);
	///*auto default_texture = CourseLab::Model::TextureFromFile("default_texture.jpg", "res/default", false);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, default_texture);*/

	//CourseLab::ShaderProgram program("res/shader");

	//auto light = new CourseLab::LightSource(0.6f, 0.2f, 1.0f, glm::vec3(1.0f));
	//light->Pos(glm::vec3(1.0f, 4.0f, 0.0f));
	//CourseLab::Scene scene(light);
	//scene.SetCamera(new CourseLab::Camera(glm::vec3(0.0f, 0.25f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), window.get(), 45.0f));

	//auto model1 = new CourseLab::Model("C:/Users/Valentine/Desktop/models/nanosuit/nanosuit.obj", time);
	////auto model2 = new CourseLab::Model("C:/Users/Valentine/Desktop/models/nanosuit/nanosuit.obj",time);

	//model1->Rot(glm::vec3(glm::radians(-90.0f),0.0f,0.0f));
	//model1->Size(glm::vec3(0.3f));
	////model1->Pos(glm::vec3(0.0f, 0.0f, 0.0f));

	//scene.AddObj(model1);
	////scene.AddObj(model2);
	//
	////GUI Init

	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;

	//ImGui::StyleColorsDark();

	//ImGui_ImplGlfw_InitForOpenGL(window->glWindow, true);
	//ImGui_ImplOpenGL3_Init(glsl_version);

	//glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_LINE);
	//glClearColor(0.7f, 0.7f, 0.8f, 1);

	//bool show_demo_window = true;
	//while (!window->shouldClose())
	//{
	//	glfwPollEvents();

	//	ImGui_ImplOpenGL3_NewFrame();
	//	ImGui_ImplGlfw_NewFrame();
	//	ImGui::NewFrame();

	//	//ImGui::ColorEdit4("Color", glm::value_ptr(color));

	//	ImGui::ShowDemoWindow(&show_demo_window);

	//	time += 0.01f;
	//	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//	// Updating Data

	//	scene.GetCamera()->Pos(glm::vec3(4.0f * cosf(time / 2), 2.0f, 4.0f * sinf(time / 2)));

	//	//Rendering

	//	program.Bind();
	//	scene.DrawScene(program.ID());

	//	ImGui::Render();
	//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//	glfwSwapBuffers(window->glWindow);
	//}

	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();

	//std::cout << "fisting is 300$";

}

void processInput(GLFWwindow* window, CourseLab::Camera& cam, float time)
{
	/*float cameraSpeed = 0.5f * time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.Translate(cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.Translate(cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.Translate(glm::vec3(-1.0f, 0.0f, 0.0f) * cameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.Translate(glm::vec3(1.0f, 0.0f, 0.0f) * cameraSpeed);*/
}
