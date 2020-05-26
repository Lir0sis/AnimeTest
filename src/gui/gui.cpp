
#include "gui.h"
#include "../objects/renderer.h"

namespace CourseLab
{
	Gui::Gui(Renderer* r) : m_renderer(r) {
		LOG("--- GUI constructor")
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		m_IO = ImGui::GetIO(); (void)m_IO;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_renderer->m_window->glWindow, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		LOG("Initialized ImGUI IO and GLFW_Impl")
	}

	Gui::~Gui() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void Gui::Update() {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}