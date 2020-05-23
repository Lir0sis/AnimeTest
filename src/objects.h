#pragma once

#include <../main.h>
#include "layout.h"
#include "vertexArray.h"
#include "window.h"

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
		ModelMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
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
			layout.PushAttrib({ 4, GL_INT, false });
			layout.PushAttrib({ 4, GL_FLOAT, false });

			vao->AddLayout(layout); vao->ActivateLayout();
		}
		~ModelMesh() { delete vao; }

		GLuint GetBaseFace() const { return indices[0]; }
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

			Bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			vao->Unbind();
			glActiveTexture(GL_TEXTURE0);
		}
	};

	class Object
	{
	private:
		const int m_ID;
		static int Counter() { static int ID{ 0 }; return ID++;}
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
		inline void Scale(const glm::vec3& scale) { m_scale *= scale; }

	};

	class IDrawabale
	{
	public:
		virtual void Draw(GLuint shaderID, glm::mat4 transform) = 0;
	};
	
	class Model : public Object, public virtual Transform, public virtual IDrawabale
	{

	public:
		struct BoneInfo {
			glm::mat4 offset;
			glm::mat4 animPoseTransform;
			BoneInfo() {
				offset = glm::mat4(0.0f);
				animPoseTransform = glm::mat4(0.0f);
			}
		};
		struct VertBoneData {
			GLint IDs[4] = {0,0,0,0};
			GLfloat weights[4] = { 0.0f,0.0f,0.0f,0.0f };

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
	protected:
		std::string m_FilePath;
		std::vector<ModelMesh*> m_Meshes;
		std::vector<ModelMesh::Texture> m_TextLoaded;
		const float& m_time;

		std::vector<BoneInfo> m_BonesInfo;
		std::map<std::string, GLint> m_BoneMap;
		GLuint m_NumBones;

		glm::mat4 m_GlobInvTransform;

		const aiScene* m_scene;
		const aiAnimation* m_Animation;
	public:
		Model(const std::string& path, float& time) : m_time(time)
		{
			Assimp::Importer importer;
			
			importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

			m_scene = importer.GetOrphanedScene();
			if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				throw new std::exception("Loading model error");
			}
			std::cout << importer.GetErrorString() << std::endl;

			m_GlobInvTransform = glm::inverse(mat4_cast(m_scene->mRootNode->mTransformation));

			if (m_scene->HasAnimations()) {
				m_Animation = m_scene->mAnimations[0];
			}
			
			m_FilePath = path.substr(0, path.find_last_of('/'));

			processNode(m_scene->mRootNode, m_scene);
		};

		~Model() {};

		void Draw(GLuint shaderID, glm::mat4 transform) override
		{ 
			std::vector<glm::mat4> boneTransforms;
			BoneTransform(m_time, boneTransforms);

			GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uBones[0]"), boneTransforms.size(), GL_FALSE, glm::value_ptr(boneTransforms[0]))); 

			GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uTransform"), 1, GL_FALSE, glm::value_ptr(transform)));
			GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uModel"), 1, GL_FALSE, glm::value_ptr( GetTransform() )));

			for (GLint i = 0; i < m_Meshes.size(); i++)
				m_Meshes[i]->Draw(shaderID);
		}

		void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
		{
			glm::mat4 identity(1.0f);

			if (!m_scene->HasAnimations()) return;
			float TicksPerSecond = (float)(m_Animation->mTicksPerSecond != 0 ? m_Animation->mTicksPerSecond : 25.0f);
			float TimeInTicks = TimeInSeconds * TicksPerSecond;
			float AnimationTime = std::fmod(TimeInTicks, (float)m_Animation->mDuration);

			readNodeHeirarchy(AnimationTime, m_scene->mRootNode, identity);

			Transforms.resize(m_NumBones);

			for (GLuint i = 0; i < m_NumBones; i++) {
				Transforms[i] = m_BonesInfo[i].animPoseTransform;
			}
		}
	private:
		const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName)
		{
			for (GLint i = 0; i < pAnimation->mNumChannels; i++) {
				const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

				if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
					return pNodeAnim;
				}
			}

			return nullptr;
		}

		void readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& bindTransform)
		{
			std::string NodeName(pNode->mName.data);

			glm::mat4 localBindTransform = mat4_cast(pNode->mTransformation);

			const aiNodeAnim* pNodeAnim = findNodeAnim(m_Animation, NodeName);

			if (pNodeAnim) {
				glm::vec3 vScale(0.0f);
				InterpolateScale(vScale, AnimationTime, pNodeAnim);
				glm::mat4 scale = glm::scale(glm::mat4(1.0f),vScale);

				glm::quat quat;
				InterpolateRot(quat, AnimationTime, pNodeAnim);
				glm::mat4 rot = glm::toMat4(quat);

				glm::vec3 vPos;
				InterpolatePos(vPos, AnimationTime, pNodeAnim);
				glm::mat4 pos = glm::translate(glm::mat4(1.0f), vPos);

				localBindTransform = pos * rot *  scale;
			}

			glm::mat4 currentPoseTransform = bindTransform * localBindTransform;

			if (m_BoneMap.find(NodeName) != m_BoneMap.end()) {
				GLint BoneIndex = m_BoneMap[NodeName];
				m_BonesInfo[BoneIndex].animPoseTransform =  m_GlobInvTransform * currentPoseTransform *m_BonesInfo[BoneIndex].offset;
			}

			for (GLint i = 0; i < pNode->mNumChildren; i++) {
				readNodeHeirarchy(AnimationTime, pNode->mChildren[i], currentPoseTransform);
			}
		}

		void InterpolateScale(glm::vec3& vec, float time, const aiNodeAnim* node) {

			for (GLuint i = 0; i < node->mNumScalingKeys - 1; i++) {
				if (time < (float)node->mScalingKeys[i + 1].mTime) {
					float delta = node->mScalingKeys[i + 1].mTime - node->mScalingKeys[i].mTime;
					float factor = (time - node->mScalingKeys[i].mTime) / delta;

					auto vec1 = vec3_cast(node->mScalingKeys[i].mValue);

					auto vec2 = vec3_cast(node->mScalingKeys[i + 1].mValue);

					vec = glm::mix(vec1, vec2, factor);
					return;
				}
				
			}
			vec = vec3_cast(node->mScalingKeys[0].mValue);
		}
		void InterpolatePos(glm::vec3& vec, float time, const aiNodeAnim* node) {

			for (GLuint i = 0; i < node->mNumPositionKeys - 1; i++) {
				if (time < (float)node->mPositionKeys[i + 1].mTime) {
					float delta = node->mPositionKeys[i + 1].mTime - node->mPositionKeys[i].mTime;
					float factor = (time - node->mPositionKeys[i].mTime) / delta;

					auto vec1 = vec3_cast(node->mPositionKeys[i].mValue);
					auto vec2 = vec3_cast(node->mPositionKeys[i + 1].mValue);

					vec = glm::mix(vec1, vec2, factor);
					return;
				}
			}
			vec = vec3_cast(node->mPositionKeys[0].mValue);

		}
		void InterpolateRot(glm::quat& quat, float time, const aiNodeAnim* node) {


			for (GLuint i = 0; i < node->mNumRotationKeys - 1; i++) {
				if (time < (float)node->mRotationKeys[i + 1].mTime) {
					float delta = node->mRotationKeys[i + 1].mTime - node->mRotationKeys[i].mTime;
					float factor = (time - node->mRotationKeys[i].mTime) / delta;

					auto quat1 = quat_cast(node->mRotationKeys[i].mValue);
					auto quat2 = quat_cast(node->mRotationKeys[i + 1].mValue);

					quat = glm::mix(quat1, quat2, factor);
					return;
				}
			}
			quat = quat_cast(node->mRotationKeys[0].mValue);

		}

		void processNode(aiNode* node, const aiScene* scene)
		{

			for (GLuint i = 0; i < node->mNumMeshes; i++) 
			{
				GLuint meshIndx = node->mMeshes[i];
				
				aiMesh* mesh = scene->mMeshes[meshIndx];
				m_Meshes.push_back(processMesh(mesh, scene, meshIndx));

			}

			for (GLuint i = 0; i < node->mNumChildren; i++) 
			{
				processNode(node->mChildren[i], scene);
			}
		}

		static inline glm::vec3 vec3_cast(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }
		static inline glm::vec2 vec2_cast(const aiVector3D& v) { return glm::vec2(v.x, v.y); } 
		static inline glm::quat quat_cast(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }
		static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose( glm::make_mat4(&m.a1)); }


		ModelMesh* processMesh(aiMesh* mesh, const aiScene* scene, GLuint meshID)
		{
			std::vector<VertBoneData> Bones;
			Bones.resize(mesh->mNumVertices);
			if (mesh->HasBones()) {
				for (int j = 0; j < mesh->mNumBones; j++) {
					GLint boneID = 0;
					std::string boneName(mesh->mBones[j]->mName.data);

					if (m_BoneMap.find(boneName) == m_BoneMap.end()) {
						boneID = m_NumBones;
						m_NumBones++;
						BoneInfo bi;
						m_BonesInfo.push_back(bi);

						m_BonesInfo[boneID].offset = mat4_cast(mesh->mBones[j]->mOffsetMatrix);
						m_BoneMap[boneName] = boneID;
					}
					else {
						boneID = m_BoneMap[boneName];
					}

					for (int k = 0; k < mesh->mBones[j]->mNumWeights; k++) {
						GLint vertID = mesh->mFaces[0].mIndices[0] +
							mesh->mBones[j]->mWeights[k].mVertexId;
						GLfloat weight = mesh->mBones[j]->mWeights[k].mWeight;

						Bones[vertID].Add(boneID, weight);
					}
				}
			}

			std::vector<ModelMesh::Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<ModelMesh::Texture> textures;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				ModelMesh::Vertex vertex;
				glm::vec3 vector;

				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;

				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;

				if (mesh->mTextureCoords[0]) 
				{
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);

				if (mesh->HasTangentsAndBitangents()) {
					vector.x = mesh->mTangents[i].x;
					vector.y = mesh->mTangents[i].y;
					vector.z = mesh->mTangents[i].z;
					vertex.Tangent = vector;

					vector.x = mesh->mBitangents[i].x;
					vector.y = mesh->mBitangents[i].y;
					vector.z = mesh->mBitangents[i].z;
					vertex.Bitangent = vector;
				}

				if (mesh->HasBones())
				{

					glm::ivec4 uvec;

					uvec.x = Bones[i].IDs[0];
					uvec.y = Bones[i].IDs[1];
					uvec.z = Bones[i].IDs[2];
					uvec.w = Bones[i].IDs[3];
					vertex.BoneIDs = uvec;
					
					glm::vec4 vec;
					vec.x = Bones[i].weights[0];
					vec.y = Bones[i].weights[1];
					vec.z = Bones[i].weights[2];
					vec.w = Bones[i].weights[3];

					vertex.Weights = vec;
				}

				vertices.push_back(vertex);
			}

			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];

				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<ModelMesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<ModelMesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			std::vector<ModelMesh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

			std::vector<ModelMesh::Texture> heightMaps = loadMaterialTextures(material,   aiTextureType_HEIGHT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

			return new ModelMesh(vertices, indices, textures);
		}

		std::vector<ModelMesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		{
			std::vector<ModelMesh::Texture> textures;
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				std::cout << type << "\t" + i << "\t" << str.C_Str() << std::endl;
				// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
				bool skip = false;
				for (unsigned int j = 0; j < m_TextLoaded.size(); j++)
				{
					if (std::strcmp(m_TextLoaded[j].path.data(), str.C_Str()) == 0)
					{
						textures.push_back(m_TextLoaded[j]);
						skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
						break;
					}
				}
				if (!skip)
				{   // if texture hasn't been loaded already, load it
					ModelMesh::Texture texture;
					texture.id = TextureFromFile(str.C_Str(), this->m_FilePath, false);
					texture.type = typeName;
					texture.path = str.C_Str();
					textures.push_back(texture);
					m_TextLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
				}
			}
			return textures;
		}

		public:
			static GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma)
			{
				std::string filename = std::string(path);
				filename = directory + '/' + filename;

				unsigned int textureID;
				glGenTextures(1, &textureID);

				int width, height, nrComponents;
				unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
				if (data)
				{
					GLenum format = GL_RGBA8;
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

	class Camera : public Object, virtual public Transform
	{
	private:
		glm::vec3 m_Up, m_Right, m_Direction;
		glm::vec3 m_Target;
		float m_fov, m_zNear, m_zFar;
		AppWindow* window;
	public:
		Camera(glm::vec3 position, glm::vec3 target, AppWindow* window, float fov = 45.0f) {

			Pos(position);

			m_fov = fov;
			m_zNear = 0.1f;
			m_zFar = 100.0f;

			m_Target = target;

			m_Direction = glm::normalize(position - m_Target);
			m_Right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), m_Direction));
			m_Up = glm::cross(m_Direction, m_Right);
			
			this->window = window;
		}
		~Camera() {}

		glm::mat4 GetView() {

			return glm::lookAt(Pos(), m_Target, m_Up);
		}

		void SetFOV(float fov) {

			m_fov = fov;
		}

		glm::mat4 GetProj() {

			GLint width, height;
			window->GetSize(width, height);

			return glm::perspective(m_fov, (float)width/ (float)height, m_zNear, m_zFar);
		}

		glm::mat4 GetCamMatrix() {

			return GetProj() * GetView();
		}
	};

	class LightSource : public Object, public virtual Transform
	{
	public:
		struct LightParams 
		{
			LightParams() { ambient = 1; diffuse = 1; specular = 1; }
			LightParams(float ambient, float diffuse, float specular)
			{
				this->ambient = ambient;
				this->diffuse = diffuse;
				this->specular= specular;
			}

			float ambient;
			float diffuse;
			float specular;
		};

	private:
		glm::vec3 m_Color;
		LightParams params;
	public:
		LightSource(float ambient, float diffuse, float specular, 
			glm::vec3 color = glm::vec3(1.0f)) : m_Color(glm::vec3(color))
		{
			params = LightParams(ambient, diffuse, specular);
		}
		LightSource(LightParams params, glm::vec3 color = glm::vec3(1.0f)) : m_Color(color)
		{
			this->params = params;
		}
		~LightSource() {}

		void SetColor(glm::vec3 color) 
		{
			m_Color = color;
		}

		void SendData(GLint shaderID,glm::vec3 eyePos) 
		{
			GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.position"), 1, glm::value_ptr(Pos())));
			GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.viewPos"), 1, glm::value_ptr(eyePos)));
			GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.ambient"), 1, glm::value_ptr(m_Color * params.ambient)));
			GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.diffuse"), 1, glm::value_ptr(m_Color * params.diffuse)));
			GLCall(glUniform3fv(glGetUniformLocation(shaderID, "light.specular"), 1, glm::value_ptr(m_Color * params.specular)));

		}
	};
}