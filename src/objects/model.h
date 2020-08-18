#pragma once

#include <../main.h>
#include "../layout.h"
#include "../vertexArray.h"
#include "../animation/animation.h"
#include "interfaces.h"

namespace CourseLab
{
	class Renderer;
	class Gui;

	class ModelMesh
	{
	public:
		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;

			glm::vec3 Tangent;
			glm::vec3 Bitangent;
			glm::ivec4 BoneIDs;
			glm::vec4 Weights;

			Vertex() {
				Position = glm::vec3(0.0f);
				Normal = glm::vec3(0.0f);
				TexCoords = glm::vec2(0.0f);
				Tangent = glm::vec3(0.0f);
				Bitangent = glm::vec3(0.0f);
				BoneIDs = glm::ivec4(0.0f);
				Weights = glm::vec4(0.0f);
			}
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
		ModelMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
		~ModelMesh();

		GLuint GetBaseFace() const;
		void Bind() const;

		void Draw(GLuint shaderID);
	};

	class Model : public virtual Object
	{
	protected:
		std::string m_FilePath;
		std::vector<ModelMesh*> m_Meshes;
		std::vector<ModelMesh::Texture> m_TextLoaded;
		Renderer* m_renderer;

		std::shared_ptr<Joint> m_root = nullptr;
		std::shared_ptr<AnimationLoader> m_aLoder;
		glm::mat4 m_InvBindTransform;

		const aiScene* m_scene;
	public:

		Model(const std::string& path, Renderer* r);

		~Model();

		void Update() override;

		void Draw(GLuint shaderID);

		void GetBoneTransforms(float time, std::vector<glm::mat4>& out);

	private:
		struct VertBoneData
		{
			GLint IDs[4] = { 0, 0, 0, 0 };
			GLfloat weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			void Add(GLint boneID, GLfloat weight) {
				for (int i = 0; i < 4; i++) {
					if (weights[i] == 0.0) {
						IDs[i] = boneID;
						weights[i] = weight;
						return;
					}
				}
			}
		};

		std::map<std::string, Bone> _boneMap;
		std::set<std::string> _bones;
		GLuint m_NumBones;

		void getAllBones(aiNode* start);
		void orderBones(aiNode* rootNode);
		aiNode* searchModelRoot(aiNode* start);
		std::shared_ptr<Joint> LoadBones(aiNode* node);

		void processNode(aiNode* node, const aiScene* scene);
		ModelMesh* processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<ModelMesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	public:
		static GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma);

		friend class Gui;
	};
}