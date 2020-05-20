#pragma once

#include <../main.h>
#include "objects.h"

namespace CourseLab 
{
	class Scene 
	{
	private:
		std::vector<Model*> objects;
		Camera* activeCamera;
		LightSource* lightSrc;
	public:
		Scene(LightSource* light) : activeCamera(nullptr), lightSrc(light) {}
		~Scene() {}

		void AddObj(Model* obj) {
			objects.push_back(obj);
		}

		Camera* GetCamera() {
			return activeCamera;
		}

		void SetCamera(Camera* cam) {
			activeCamera = cam;
		}
		
		void DrawScene(GLuint shaderID) 
		{
			lightSrc->SendData(shaderID, activeCamera->Pos());

			for (auto& obj : objects) 
			{
				obj->Draw(shaderID, activeCamera->GetCamMatrix());
			}
		}
	};
}