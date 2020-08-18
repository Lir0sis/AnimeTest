#pragma once

#include <../main.h>
#include "objects/camera.h"
#include "objects/light.h"

namespace CourseLab 
{
	class Renderer;
	class Model;

	class Scene 
	{
	public:
		// duration in seconds
		GLfloat m_duration; 
		GLfloat m_timeOffset;

	private:
		std::vector<Model*> objects;
		Camera* m_sceneCamera;
		LightSource* m_lightSrc;
		Renderer* m_renderer;

	public:
		Scene(Renderer* r, GLfloat offset);
		~Scene();

		void AddObj(const std::string& path);

		inline Camera* GetCamera() {
			return m_sceneCamera;
		}

		inline void SetCamera(Camera* cam) { m_sceneCamera = cam; }
		inline void SetLight(LightSource* light) { m_lightSrc = light; }
		
		void Update();

		void DrawScene(GLuint shaderID);

		friend class Gui;
	};
}