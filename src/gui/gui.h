#pragma once

#include <../main.h>
//#include "../objects/renderer.h"

#include <imGui/imgui.h>
#include <imGui/imgui_impl_opengl3.h>
#include <imGui/imgui_impl_glfw.h>
#include <imGui/ext/imfilebrowser.h>


namespace CourseLab
{
	class Renderer;
	class Joint;
	class Model;

	class Gui {
	public:
		struct Selected {
			Model* model;
			std::shared_ptr<Joint> joint;
		};
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow 
			| ImGuiTreeNodeFlags_OpenOnDoubleClick 
			| ImGuiTreeNodeFlags_SpanAvailWidth;
	private:
		ImGui::FileBrowser m_fileBrowser;
		Selected m_joint_id;
		Renderer* m_renderer;
		ImGuiIO m_IO;

	public:
		Gui(Renderer* r);
		~Gui();

		void Update();

	private:
		void drawScene();
		void drawJointH(std::shared_ptr<Joint>& joint, Model*& obj);
	};
}