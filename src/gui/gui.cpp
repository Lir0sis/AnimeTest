#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

#include "gui.h"
#include "objects/renderer.h"
#include "animation/animation.h"
#include "objects/timeline.h"
#include "scene.h"
#include "objects/model.h"


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

		m_fileBrowser.SetTitle("Model Loading");
		m_fileBrowser.SetTypeFilters({ ".obj", ".dae", "." });

		LOG("Initialized ImGUI IO and GLFW_Impl")
	}

	Gui::~Gui() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void Gui::Update() {

		bool truee = true;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					m_fileBrowser.SetPwd("E:/Valik/models/");
					m_fileBrowser.Open();
					//m_renderer->m_activeScene->AddObj("E:/Valik/models/thin matrix/model2.dae");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		m_fileBrowser.Display();
		if (m_fileBrowser.HasSelected()) {
		if (m_renderer->m_activeScene != nullptr) {
				auto path = m_fileBrowser.GetSelected();
				m_renderer->m_activeScene->AddObj(path.string());
				std::cout << "PATH:" << m_fileBrowser.GetSelected().string() << std::endl;
				//m_renderer->m_activeScene->AddObj("C:/Users/Valentine/Desktop/models/thin matrix/model.dae");
			}
			m_fileBrowser.ClearSelected();
		}

		std::string name("Current time: " + std::to_string(m_renderer->GetCurrentFrame()));

		drawScene();


		ImGui::Begin("Controls");
		ImGui::Text(name.c_str());
		ImGui::NextColumn();

		ImGui::Checkbox("Repeat", &(m_renderer->m_timeline->m_isRepeating));
		ImGui::Columns(3, "myColumns3", false);

		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Play")) {
			m_renderer->m_timeline->Play();
		} ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::Button("Pause")) {
			m_renderer->m_timeline->Pause();
		} ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::Button("Stop")) {
			m_renderer->m_timeline->Stop();
		} ImGui::NextColumn();
		ImGui::Separator(); ImGui::Columns(1);

		ImGui::DragFloat("Speed factor", &m_renderer->m_timeline->m_speedFactor, 0.01f, 0.1f, 3.0f);


		static int choice = 0;
		ImGui::Columns(3, "myColumn3", false);
		ImGui::RadioButton("Position", &choice, 0); ImGui::NextColumn();
		ImGui::RadioButton("Rotation", &choice, 1); ImGui::NextColumn();
		ImGui::RadioButton("Scale", &choice, 2); ImGui::NextColumn();
		ImGui::Separator(); ImGui::Columns(1);

		glm::vec3 slider(0.0f);
		if (m_joint_id.joint != nullptr) {
			if (choice == 0) {

				if (ImGui::SliderFloat("Pos: X", &slider.x, -20, 20) ||
					ImGui::SliderFloat("Pos: Y", &slider.y, -20, 20) ||
					ImGui::SliderFloat("Pos: Z", &slider.z, -20, 20)) {
					m_joint_id.joint->setPoseVec(m_renderer->m_timeline->m_frameStamp,slider);
				}
			}
			else if (choice == 1) {

				ImGui::SliderAngle("Rot: Yaw", &slider.x);
				ImGui::SliderAngle("Rot: Pitch", &slider.y);
				ImGui::SliderAngle("Rot: Roll", &slider.z);
				m_joint_id.joint->setRotVec(m_renderer->m_timeline->m_frameStamp, slider);
			}
			else if (choice == 2) {

				ImGui::SliderFloat("Scale: X", &slider.x, 0.1f, 4);
				ImGui::SliderFloat("Scale: Y", &slider.y, 0.1f, 4);
				ImGui::SliderFloat("Scale: Z", &slider.z, 0.1f, 4);
				m_joint_id.joint->setScaleVec(m_renderer->m_timeline->m_frameStamp, slider);
			}
		}

		//TODO: recieving values from frames and setting them correctly

		ImGui::End();

		ImGui::Begin("TimeLine");
		ImGui::SetNextItemWidth(-1);
		if (ImGui::SliderInt("", &m_renderer->m_timeline->m_frameStamp, 0,
			std::floor(m_renderer->m_timeline->m_duration * m_renderer->m_timeline->m_fps)))
		{
			m_renderer->m_timeline->synchroniseStamps();
		}
		ImGui::End();
		/*ImGui::Begin("Test");
		if(m_joint_id.joint !=nullptr && m_joint_id.model != nullptr)
			ImGui::Text((m_joint_id.model->m_root->m_name+ " : " + m_joint_id.joint->m_name).c_str());
		ImGui::End();*/

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void Gui::drawScene()
	{
		ImGui::Begin("Current Scene");
		auto currscene = m_renderer->m_activeScene;
		if (currscene != nullptr) {
			for (auto& obj : m_renderer->m_activeScene->objects)
			{
				ImGui::PushID(obj->GetID());
				drawJointH(obj->m_root, obj);
				ImGui::PopID();
			}
		}
		else
			ImGui::Text("Nothing to see here, m8, not even a scene...");

		ImGui::End();
	}
	void Gui::drawJointH(std::shared_ptr<Joint>& joint, Model*& obj) {

		static ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_OpenOnDoubleClick
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		static int selection_mask = (1 << 2);
		const bool is_selected = (selection_mask & (1 << joint->m_id)) != 0;

		auto node_flags = flags;

		/*if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;*/

		ImGui::PushID(joint->m_id);
		if (joint->m_children.size() > 0) {
			if (ImGui::TreeNodeEx((void*)(intptr_t)joint->m_id, node_flags, joint->m_name.c_str()))
			{

				if (ImGui::IsItemClicked())
				{
					m_joint_id = { obj, joint };
					node_flags |= ImGuiTreeNodeFlags_Selected;
				}
				//ImGui::OpenPopupOnItemClick();
				for (auto& child : joint->m_children) {
					drawJointH(child, obj);
				}

				ImGui::TreePop();
			}
		}
		else
		{
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)(intptr_t)joint->m_id, node_flags, joint->m_name.c_str());
			if (ImGui::IsItemClicked())
				m_joint_id = { obj, joint };
		}
		ImGui::PopID();
	}
}
