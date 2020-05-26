
#include "camera.h"
#include "renderer.h"

namespace CourseLab 
{
	using vec = glm::vec3;
	Camera::Camera(const std::string& name, Renderer* r, float fov = 45.0f) {

		m_renderer = r;
		m_root = std::make_shared<Joint>(name, 0, glm::mat4(1.0f), glm::mat4(1.0f));

		m_fov = fov;
		m_zNear = 0.1f;
		m_zFar = 100.0f;

		m_Target = vec(0.0f, 0.0f, 0.0f);

		m_Direction = glm::normalize(vec(4.0f, 0.0f, -4.0f) - m_Target);
		m_Right = glm::normalize(glm::cross(vec(0.0f, 1.0f, 0.0f), m_Direction));
		m_Up = glm::cross(m_Direction, m_Right);

		LOG("--- Obj --- Created Camera")

	}
	Camera::~Camera() {}

	glm::vec3 Camera::GetPos()
	{
		return m_root->GetPoseVec(m_renderer->m_timeline->GetCurrentFrame());
	}

	glm::mat4 Camera::GetView() {

		GLfloat time = m_renderer->GetCurrentFrame()/m_renderer->m_timeline->GetFPS();

		return glm::lookAt(vec(4.0f* std::sinf(time), 0.0f, 4.0f * std::cos(time)), m_Target, m_Up);
	}

	void Camera::SetFOV(float fov) {

		m_fov = fov;
	}

	glm::mat4 Camera::GetProj() {

		GLint width, height;
		m_renderer->m_window->GetSize(width, height);

		if (!width || !height)
			return glm::mat4(1.0f);

		return glm::perspective(m_fov, (float)width / (float)height, m_zNear, m_zFar);
	}

	glm::mat4 Camera::GetCamMatrix() {

		return GetProj() * GetView();
	}

	void Camera::Update() {

		GLCall(glUniformMatrix4fv(
			glGetUniformLocation(m_renderer->GetShaderID(), "uTransform"),
			1, 
			GL_FALSE, 
			glm::value_ptr(GetCamMatrix()))
		);
	}
}