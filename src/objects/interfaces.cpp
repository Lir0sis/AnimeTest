
#include "interfaces.h"

namespace CourseLab
{
	Transform::Transform() : m_pos(glm::vec3(0.0f)), m_rot(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)) {}
	Transform::~Transform() {}

	const glm::mat4 Transform::GetTransform()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_pos);

		transform = glm::rotate(transform, m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

		transform = glm::scale(transform, m_scale);

		return transform;
	}
}