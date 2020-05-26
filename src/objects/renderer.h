#pragma once
#include <../main.h>

#include "../window.h"
#include "../scene.h"
#include "../gui/gui.h"
#include "../shader.h"
#include "camera.h"
#include "timeline.h"

namespace CourseLab
{
	class Renderer {
	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<AppWindow> m_window;
		std::shared_ptr<Gui> m_gui;	

		Camera* m_editor_cam;
		Camera* m_activeCam;
		
		TimeLine* m_timeline;
		std::shared_ptr<ShaderProgram> m_shader;

		
	public:
		Renderer(const Renderer&) = delete;
		const Renderer& operator=(const Renderer&) = delete;

		static Renderer* Instance();

		void Update();

		inline GLuint GetShaderID() { return m_shader->ID(); }
		inline Camera* GetActiveCamera() { return m_activeCam; }
		inline GLfloat GetCurrentFrame() { return m_timeline->GetCurrentFrame(); }
		inline bool ShouldStop() { return m_window->shouldClose(); }
	private:
		inline void MakeActiveCamera() {

		}

		Renderer();
		~Renderer();

		friend class Gui;
		friend class Scene;
		friend class Camera;
	};
}