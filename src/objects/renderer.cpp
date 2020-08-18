
#include "renderer.h"
#include "model.h"

namespace CourseLab
{
	void Renderer::setActiveScene()
	{
		auto time = m_timeline->GetCurrentFrame();
		auto fps = m_timeline->GetFPS();
		for (auto& scene : m_scenes) {
			if (scene->m_timeOffset * fps + scene->m_duration * fps > time)
			{
				if (time < scene->m_timeOffset * fps)
					break;
				m_activeScene = scene;
				return;
			}
		}
		m_activeScene = 0;
	}
	Renderer::Renderer()
	{
		m_window = std::make_shared<AppWindow>(1280, 720);

		auto default_texture = Model::TextureFromFile("default_texture.jpg", "res/default", false);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, default_texture);

		m_shader = std::make_shared<ShaderProgram>("res/shader");
		m_shader->Bind();

		m_timeline = TimeLine::Instance();
		m_activeCam = m_editor_cam = new Camera(std::string("MainCamera"), this, 45.0f);

		auto scene = std::make_shared<Scene>(this, 0.0f);
		scene->SetCamera(new Camera(std::string("SceneCamera"), this, 45.0f));
		scene->SetLight(new LightSource(this, { 0.5f, 1.0f, 0.2f }));

		m_scenes.push_back(scene);
		m_activeScene = scene;

		m_gui = std::make_shared<Gui>(this);

	}
	Renderer* Renderer::Instance()
	{
		static Renderer instance;
		return &instance;
	}

	Renderer::~Renderer() {

		m_shader.reset();
		m_gui.reset();

		delete m_editor_cam;

		for(auto& scene : m_scenes)
			scene.reset();

		m_window.reset();
	}
	void Renderer::Update()
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_timeline->Update();
		m_shader->Bind();
		m_activeCam->Update();

		/*for (auto& scene : m_scenes) {
			scene->Update();
		}*/
		if(m_activeScene)
			m_activeScene->Update();

		m_gui->Update();

		glfwSwapBuffers(m_window->glWindow);
	}
}