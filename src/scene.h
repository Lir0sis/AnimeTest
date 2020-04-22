#pragma once

#include <../main.h>
#include "objects.h"

namespace CourseLab 
{
	class Scene 
	{
	private:
		std::vector<Object*> objects;
		Camera* activeCamera;
	public:
		Scene() : activeCamera(nullptr) {}
		~Scene() {}

		void AddObj(Object* obj) 
		{
			objects.push_back(obj);
		}

		Camera* GetCamera() 
		{
			return activeCamera;
		}
		
		void DrawScene() 
		{

		}
	};
}