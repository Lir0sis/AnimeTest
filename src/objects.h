#pragma once

#include <../main.h>
#include "dataStructs/texture.h"
#include "dataProcessing/parser.h"


namespace CourseLab 
{
	class ModelData 
	{
	private:
		objl::Mesh m_Mesh;
		VertexArray* vao;
	public:
		ModelData(const objl::Mesh& mesh) : m_Mesh(mesh)
		{
			const GLfloat* vertex = Parser::GetVertexBuffer(mesh.Vertices);
			const GLuint* index = Parser::GetIndexBuffer(mesh.Indices);

			vao = new VertexArray();

			const IndexBuffer* smthI = new IndexBuffer(sizeof(GLuint) * mesh.Indices.size(), index);
			const VertexBuffer* smthV = new VertexBuffer(sizeof(GLfloat) * mesh.Vertices.size() * 8, vertex);

			vao->BindBuffers(smthI, smthV);

			VertexLayout layout; layout.SetDefault();
			vao->AddLayout(layout); vao->ActivateLayout();
		}
		~ModelData() { delete vao; }

		objl::Mesh& GetMesh() { return m_Mesh; }
		void Bind() const { vao->Bind(); }
	};

	class Object 
	{
	private:
		const int m_ID;
		static int Counter() { static int ID{ 0 }; return ++ID;}
	public:
		Object() : m_ID(Counter()) {  }
		~Object() {}

		
		const int GetID() const { return m_ID; }
	};

	class Transform 
	{	
	private:
		glm::vec3 m_pos;
		glm::vec3 m_rot;
		glm::vec3 m_scale;
	public:
		Transform() : m_pos(glm::vec3(0.0f)), m_rot(glm::vec3(0.0f)), m_scale(glm::vec3(1.0f)) {}
		~Transform() {}


		inline glm::vec3 Pos() { return m_pos; }
		inline glm::vec3 Rot() { return m_rot; }
		inline glm::vec3 Size() { return m_scale; }

		inline void Pos(const glm::vec3& pos) { m_pos = pos; }
		inline void Rot(const glm::vec3& rot) { m_rot = rot; }
		inline void Size(const glm::vec3& scale) { m_scale = scale; }

		const glm::mat4 GetTransform() 
		{ 
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_pos);

			transform = glm::rotate(transform, m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, m_rot.z, glm::vec3(0.0f, 0.0f, 1.0f));

			transform = glm::scale(transform, m_scale);

			return transform;
		}

		inline void Translate(const glm::vec3& pos) { m_pos += pos; }
		inline void Rotate(const glm::vec3& rot) { m_rot += rot; }
		inline void Scale(const glm::vec3& scale) { m_rot *= scale; }

	};

	class IDrawabale
	{
	public:
		virtual void Draw(GLuint shaderID, glm::mat4 transform) = 0;
	};
	
	class Model : public Object, public virtual Transform, public IDrawabale
	{
	private:
		objl::Loader* data;
		std::vector<ModelData*> meshes;
		Texture* texture;
		
	public:
		Model(File* texture, const std::string& path) : texture(nullptr)
		{
			//this->texture = new Texture(texture);
			data = new objl::Loader();
			if (!data->LoadFile(path))
				throw new std::exception("Couldn't load file");

			for(int i = 0; i < data->LoadedMeshes.size(); i++)
			{
				meshes.push_back(new ModelData(data->LoadedMeshes[i]));
			}
		};

		~Model() 
		{
			delete data;
			delete texture;
		};

		void Draw(GLuint shaderID, glm::mat4 transform) override
		{ 
			GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uTransform"), 1, GL_FALSE, glm::value_ptr(transform)));
			GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uModel"), 1, GL_FALSE, glm::value_ptr( GetTransform() )));
			
			for (auto& mesh : meshes) 
			{
				mesh->Bind();
				objl::Mesh& tMesh = mesh->GetMesh();
				GLCall(glUniform3f(glGetUniformLocation(shaderID, "uMaterial.ambient"),
					tMesh.MeshMaterial.Ka.X, tMesh.MeshMaterial.Ka.Y, tMesh.MeshMaterial.Ka.Z));
				GLCall(glUniform3f(glGetUniformLocation(shaderID, "uMaterial.diffuse"),
					tMesh.MeshMaterial.Kd.X, tMesh.MeshMaterial.Kd.Y, tMesh.MeshMaterial.Kd.Z));
				GLCall(glUniform3f(glGetUniformLocation(shaderID, "uMaterial.specular"),
					tMesh.MeshMaterial.Ks.X, tMesh.MeshMaterial.Ks.Y, tMesh.MeshMaterial.Ks.Z));
				GLCall(glUniform1f(glGetUniformLocation(shaderID, "uMaterial.shininess"), tMesh.MeshMaterial.Ns));

				GLCall(glDrawElements(GL_TRIANGLES, mesh->GetMesh().Indices.size(), GL_UNSIGNED_INT, 0));
			}
		}
	};

	class Camera : public Object, virtual public Transform
	{
	private:
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_Direction;
	public:
		glm::vec3 Target;
		Camera(glm::vec3 position, glm::vec3 target = glm::vec3(0.0f))
		{
			Pos(position);
			Target = target;
			m_Direction = glm::normalize(position - Target);
			m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Direction));
			m_Up = glm::cross(m_Direction, m_Right);
		}
		~Camera() {};

		glm::mat4 GetViewMatrix() 
		{
			return glm::lookAt(Pos(), Target, m_Up);
		}
	};

	class LightSource 
	{
	private:

	public:
	};
}