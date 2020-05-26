
#include "model.h"
#include "renderer.h"


namespace CourseLab
{
	ModelMesh::ModelMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
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
	ModelMesh::~ModelMesh() { delete vao; }

	GLuint ModelMesh::GetBaseFace() const { return indices[0]; }
	void ModelMesh::Bind() const { vao->Bind(); }

	void ModelMesh::Draw(GLuint shaderID)
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

	Model::Model(const std::string& path, Renderer* r) : m_renderer(r)
	{
		//LOG("--- Obj --- Model Constructor")

		Assimp::Importer importer;
		importer.ReadFile(path,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
		m_scene = importer.GetOrphanedScene();

		if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			throw new std::exception("Loading model error");
		}
		std::cout << importer.GetErrorString() << std::endl;
		LOG("--- Obj --- Model imported scene (assimp)")

		m_FilePath = path.substr(0, path.find_last_of('/'));
		m_aLoder = std::make_shared<AnimationLoader>(m_scene);

		//m_BoneMap.insert(std::pair<std::string, Bone>(std::string("root"), {}));	

		auto* smth = searchModelRoot(m_scene->mRootNode);
		m_root = LoadBones(smth);
		LOG("--- Obj --- Model loaded bones")

		processNode(m_scene->mRootNode, m_scene);
		LOG("--- Obj --- Model processed Mesh")

		m_InvBindTransform = glm::inverse(m_root->GetTransform());

		m_aLoder->SetBoneMap(_boneMap);
		m_aLoder->LoadIntoBones(m_root, 0);
		LOG("--- Obj --- Model loaded anim -> bones")
	};
	Model::~Model() {};

	void Model::Update() {
		Draw(m_renderer->GetShaderID());
	}
	void Model::Draw(GLuint shaderID)
	{
		std::vector<glm::mat4> boneTransforms;
		boneTransforms.resize(m_NumBones);
		GetBoneTransforms(m_renderer->GetCurrentFrame(), boneTransforms);

		GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uBones[0]"), 
			boneTransforms.size(), GL_FALSE, glm::value_ptr(boneTransforms[0])
		));

		GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uRoot"), 
			1, GL_FALSE, glm::value_ptr(m_root->GetFrameTransform(m_renderer->GetCurrentFrame()))
		));

		/*GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderID, "uTransform"), 
			1, GL_FALSE, glm::value_ptr(m_renderer->GetActiveCamera()->GetCamMatrix())
		));*/

		for (GLint i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i]->Draw(shaderID);
	}

	void Model::GetBoneTransforms(float time, std::vector<glm::mat4>& out) {

		glm::mat4 identity(1.0f);
		m_root->GetCurrentPoseArray(m_aLoder->GetAnimTime(time), identity, out, m_InvBindTransform);
	};

	void Model::getAllBones(aiNode* start) {

		for (int i = 0; i < start->mNumChildren; i++) {

			auto child = start->mChildren[i];
			for (int j = 0; j < child->mNumMeshes; j++) {

				auto mesh = m_scene->mMeshes[child->mMeshes[j]];
				if (mesh->HasBones()) {
					for (int k = 0; k < mesh->mNumBones; k++) {
						std::string boneName(mesh->mBones[k]->mName.data);
						if (_bones.find(boneName) == _bones.end())
							_bones.insert(boneName);
					}
				}
			}
			Model::getAllBones(child);
		}
	}

	void Model::orderBones(aiNode* rootNode) {

		for (int i = 0; i < rootNode->mNumChildren; i++) {

			auto child = rootNode->mChildren[i];
			for (int j = 0; j < child->mNumMeshes; j++) {

				auto mesh = m_scene->mMeshes[child->mMeshes[j]];
				if (mesh->HasBones()) {
					for (int k = 0; k < mesh->mNumBones; k++) {

						GLint boneID = 0;
						std::string boneName(mesh->mBones[k]->mName.data);

						if (_boneMap.find(boneName) == _boneMap.end())
						{
							boneID = m_NumBones++;
							_boneMap[boneName].id = boneID;
						}
						_boneMap[boneName].offset = mat4_cast(mesh->mBones[k]->mOffsetMatrix);
					}
				}
			}
			orderBones(child);
		}
	}
	aiNode* Model::searchModelRoot(aiNode* start) {
		aiNode* out = nullptr;

		for (GLuint i = 0; i < start->mNumChildren; i++)
		{
			auto child = start->mChildren[i];
			if (_bones.size() == 0 && child->mNumMeshes > 0)
			{
				getAllBones(m_scene->mRootNode);
				//if (_bones.size() == 0) _bones.insert(start->mName.data);

				if (child->mParent != nullptr && _bones.size() > 0)
					out = searchModelRoot(m_scene->mRootNode);
				else
				{
					out = start;
					_boneMap[start->mName.data] = {
						(GLint)m_NumBones++,
						glm::inverse(mat4_cast(start->mTransformation))
					};
				}
			}

			if (_bones.find(child->mName.data) != _bones.end())
			{
				std::string name = start->mName.data;
				if (name.empty()) name = "m_Root";

				_boneMap[name] = {
					(GLint)m_NumBones++,
					glm::inverse(mat4_cast(start->mTransformation))
				};

				orderBones(m_scene->mRootNode);
				return start;

			}

			if(!out) 
				out = searchModelRoot(child);
			if(out)
				break;
		}
		return out;
	}
	std::shared_ptr<Joint> Model::LoadBones(aiNode* node) {

		std::shared_ptr<Joint> root;
		std::string name(node->mName.data);
		if (_boneMap.find(name) != _boneMap.end())
			root = std::make_shared<Joint>(name, _boneMap[name].id, 
				mat4_cast(node->mTransformation), _boneMap[name].offset);
		/*else
			root = std::make_shared<Joint>(name, m_NumBones++, mat4_cast(node->mTransformation), glm::mat4(1.0f));*/

		for (int i = 0; i < node->mNumChildren; i++)
			if (node->mChildren[i]->mNumMeshes == 0)
				root->AddChild(LoadBones(node->mChildren[i]));

		return root;

	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{

		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh*& mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}

		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	ModelMesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<VertBoneData> Bones;
		Bones.resize(mesh->mNumVertices);

		if (mesh->HasBones()) {
			for (int j = 0; j < mesh->mNumBones; j++) {
				GLint boneID = _boneMap[mesh->mBones[j]->mName.data].id;

				for (int k = 0; k < mesh->mBones[j]->mNumWeights; k++) {
					GLint vertID = mesh->mFaces[0].mIndices[0] +
						mesh->mBones[j]->mWeights[k].mVertexId;
					GLfloat weight = mesh->mBones[j]->mWeights[k].mWeight;

					Bones[vertID].Add(boneID, weight);
				}
			}
		}
		else {
			for (int i = 0; i < Bones.size(); i++) {
				Bones[i].Add(0, 1.0f);
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

			glm::ivec4 ivec;

			ivec.x = Bones[i].IDs[0];
			ivec.y = Bones[i].IDs[1];
			ivec.z = Bones[i].IDs[2];
			ivec.w = Bones[i].IDs[3];
			vertex.BoneIDs = ivec;

			glm::vec4 vec;
			vec.x = Bones[i].weights[0];
			vec.y = Bones[i].weights[1];
			vec.z = Bones[i].weights[2];
			vec.w = Bones[i].weights[3];

			vertex.Weights = vec;

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

		std::vector<ModelMesh::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return new ModelMesh(vertices, indices, textures);
	}
	std::vector<ModelMesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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

	GLuint Model::TextureFromFile(const char* path, const std::string& directory, bool gamma)
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


}