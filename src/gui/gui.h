#pragma once

#include <../main.h>
//#include "../objects/renderer.h"

#include <imGui/imgui.h>
#include <imGui/imgui_impl_opengl3.h>
#include <imGui/imgui_impl_glfw.h>


namespace CourseLab
{
	class Renderer;

	class Gui {
	public:
		enum Types
		{
			BAR = 0
		};
	private:
		Renderer* m_renderer;
		ImGuiIO m_IO;

	public:
		Gui(Renderer* r);
		~Gui();

		void Update();
	};
}