#pragma once

#include "./object.h"

namespace CourseLab
{
	class ModelMesh
	{
	public:
		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;

			glm::vec3 Tangent;
			glm::vec3 Bitangent;
		};
		struct Texture {
			GLuint id;
			std::string type;
			std::string path;
		};

	private:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		VertexArray* vao;
	public:
		ModelMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			vao = new VertexArray();

			vao->BindBuffers(new IndexBuffer(sizeof(GLuint) * indices.size(), &indices[0]),
				new VertexBuffer(sizeof(Vertex) * vertices.size(), &vertices[0]));

			VertexLayout layout;

			layout.PushAttrib({ 3 ,GL_FLOAT,false }); //pos
			layout.PushAttrib({ 3 ,GL_FLOAT,false }); //normals
			layout.PushAttrib({ 2 ,GL_FLOAT,false }); //txCoords
			layout.PushAttrib({ 3 ,GL_FLOAT,false }); //tan
			layout.PushAttrib({ 3 ,GL_FLOAT,false }); //Bitan

			vao->AddLayout(layout); vao->ActivateLayout();
		}
		~ModelMesh() { delete vao; }

		void Bind() const { vao->Bind(); }

		void Draw(GLuint shaderID)
		{
			unsigned int diffuseNr = 0;
			unsigned int specularNr = 0;
			unsigned int normalNr = 0;
			unsigned int heightNr = 0;
			if (textures.size() > 0)
			{
				for (unsigned int i = 0; i < textures.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);

					std::string number;
					std::string name = textures[i].type;
					if (name == "texture_diffuse")
						number = std::to_string(++diffuseNr);
					else if (name == "texture_specular")
						number = std::to_string(++specularNr);
					else if (name == "texture_normal")
						number = std::to_string(++normalNr);
					else if (name == "texture_height")
						number = std::to_string(++heightNr);

					glUniform1i(glGetUniformLocation(shaderID, (name + number).c_str()), i);
					glBindTexture(GL_TEXTURE_2D, textures[i].id);
				}
			}
			/*else
			{
				glActiveTexture(GL_TEXTURE0);
				glUniform1i(glGetUniformLocation(shaderID, "texture_diffuse1"), 0);
				glUniform1i(glGetUniformLocation(shaderID, "texture_specular1"), 0);
				glUniform1i(glGetUniformLocation(shaderID, "texture_normal1"), 0);
				glUniform1i(glGetUniformLocation(shaderID, "texture_height1"), 0);
				glBindTexture(GL_TEXTURE_2D, 1);
			}*/

			Bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			vao->Unbind();
			glActiveTexture(GL_TEXTURE0);
		}
	};
}