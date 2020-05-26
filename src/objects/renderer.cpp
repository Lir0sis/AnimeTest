
#include "renderer.h"
#include "model.h"

namespace CourseLab
{
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

		auto scene = std::make_shared<Scene>(this);
		scene->SetCamera(new Camera(std::string("SceneCamera"), this, 45.0f));
		scene->SetLight(new LightSource(this, { 0.5f, 1.0f, 0.2f }));

		scene->AddObj("C:/Users/Valentine/Desktop/models/thin matrix/model.dae");
		m_scenes.push_back(scene);

		m_gui = std::make_shared<Gui>(this);
		m_timeline->Play();

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

		for (auto& scene : m_scenes) {
			scene->Update();
		}

		m_gui->Update();

		glfwSwapBuffers(m_window->glWindow);
	}
}