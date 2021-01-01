#pragma once

#include <main.h>

#include "interfaces.h"
#include "animation/animation.h"
//#include "renderer.h"

namespace CourseLab
{
	class Renderer;

	class Camera : public virtual Object
	{
	private:
		Renderer* m_renderer;
		std::shared_ptr<Joint> m_root;

		glm::vec3 m_Up, m_Right, m_Direction;
		glm::vec3 m_Target;
		float m_fov, m_zNear, m_zFar;
		//std::shared_ptr<AppWindow> window;
	public:
		Camera(const std::string& name, Renderer* r, float fov);
		~Camera();

		glm::vec3 GetPos();
		glm::mat4 GetView();
		void SetFOV(float fov);

		glm::mat4 GetProj();

		glm::mat4 GetCamMatrix();

		void Update() override;

	};
}
