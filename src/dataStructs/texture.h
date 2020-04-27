#pragma once

#include <../main.h>
#include <objLoader.h>

namespace CourseLab
{
	class Texture
	{
		struct Map
		{
			Map(GLuint id, const std::string& type, const std::string& path)
			{
				this->id = id;
				this->type = type;
				this->path = path;
			}

			GLuint id;
			std::string type;
			std::string path;
		};

		struct Maps
		{
			Map diffuse;
			Map specular;
			Map ambient;
		};
	private:
		objl::Material m_Material;
	public:
		Texture(objl::Material material)
		{
			m_Material = material;
		}
		~Texture() {};

		unsigned int loadTexture(char const* path)
		{
			unsigned int textureID;
			glGenTextures(1, &textureID);

			int width, height, nrComponents;
			unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
			if (data)
			{
				GLenum format;
				if (nrComponents == 1)
					format = GL_RED;
				else if (nrComponents == 3)
					format = GL_RGB;
				else if (nrComponents == 4)
					format = GL_RGBA;

				glBindTexture(GL_TEXTURE_2D, textureID);
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(data);
			}
			else
			{
				std::cout << "Texture failed to load at path: " << path << std::endl;
				stbi_image_free(data);
			}

			return textureID;
		}

	};
}