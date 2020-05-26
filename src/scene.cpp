
#include "scene.h"
#include "objects/model.h"
#include "objects/renderer.h"

namespace CourseLab
{
	Scene::Scene(Renderer* r) :
		m_sceneCamera(nullptr), m_lightSrc(nullptr), 
		m_duration(60.0f), m_renderer(r) 
	{
		LOG("--- Obj --- Created Scene")
	}

	Scene::~Scene() 
	{
		delete m_sceneCamera;
		delete m_lightSrc;

		for (auto& obj : objects)
			delete obj;
	}

	void Scene::AddObj(const std::string& path) {
		LOG("--- Obj --- Scene> Model creation")
		Model* obj = nullptr;
		try 
		{
			obj = new Model(path, m_renderer);
		}
		catch(...)
		{
			std::cout << "--- Error --- Model didn't load well...." << std::endl;
			return;
		}
		
		if (obj != nullptr)
		{ 
			objects.push_back(obj); 
			LOG("--- Obj --- Scene> added Model")
		}
	}

	void Scene::DrawScene(GLuint shaderID)
	{
		//m_lightSrc->SendData(shaderID, m_sceneCamera->GetPos());

		for (auto& obj : objects)
		{
			obj->Update();
		}
	}

	void Scene::Update() {
		m_lightSrc->Update();
		DrawScene(m_renderer->GetShaderID());
	}
}