
#include "light.h"
#include "renderer.h"

namespace CourseLab
{
	void LightSource::SendData(GLint shaderID, glm::vec3 eyePos)
	{
		GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.position"), 1, glm::value_ptr(m_root->GetPoseVec(m_renderer->GetCurrentFrame()))));
		GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.viewPos"), 1, glm::value_ptr(eyePos)));
		GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.ambient"), 1, glm::value_ptr(m_Color * params.ambient)));
		GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.diffuse"), 1, glm::value_ptr(m_Color * params.diffuse)));
		GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.specular"), 1, glm::value_ptr(m_Color * params.specular)));
	}

	void LightSource::Update()
	{
		SendData(m_renderer->GetShaderID(), m_renderer->GetActiveCamera()->GetPos());
	}

	LightSource::LightSource(Renderer* r ,LightParams params = LightSource::LightParams())
	{
		m_renderer = r;
		m_Color = glm::vec3(1.0f);
		this->params = params;

		auto translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 0.0f));

		m_root = std::make_shared<Joint>("Light", 0, translation, glm::inverse(translation));
		LOG("--- Obj --- Created Light src")
	}

}