#pragma once

#include <../main.h>
#include "../animation/animation.h"
#include "interfaces.h"
#include "timeline.h"

namespace CourseLab
{
	class Renderer;

	class LightSource : public virtual Object
	{
	public:
		struct LightParams
		{
			LightParams() { ambient = 1; diffuse = 1; specular = 1; }
			LightParams(float ambient, float diffuse, float specular)
			{
				this->ambient = ambient;
				this->diffuse = diffuse;
				this->specular = specular;
			}

			float ambient;
			float diffuse;
			float specular;
		};
	private:
		glm::vec3 m_Color;
		LightParams params;

		Renderer* m_renderer;

		std::shared_ptr<Joint> m_root;
	public:
		LightSource(Renderer* r, LightParams params);
		inline ~LightSource() {}

		inline void SetColor(glm::vec3 color) { m_Color = color; }

		void SendData(GLint shaderID, glm::vec3 eyePos);

		void Update() override;
	};
}