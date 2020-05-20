#version 330 core

const int MAX_BONES = 100;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexture;
layout(location = 3) in ivec4 aBoneIDs;
layout(location = 4) in vec4 aWeights;

uniform mat4 uModel;
uniform mat4 uTransform;
uniform mat4 uBones[MAX_BONES];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	/*mat4 BoneTransform  = uBones[aBoneIDs[0]] * aWeights[0];
	BoneTransform += uBones[aBoneIDs[1]] * aWeights[1];
	BoneTransform += uBones[aBoneIDs[2]] * aWeights[2];
	BoneTransform += uBones[aBoneIDs[3]] * aWeights[3];*/
	
	mat4 local_transform = /*BoneTransform **/ uModel;
	TexCoords = aTexture;
	FragPos = vec3(local_transform * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(local_transform))) * aNormal;

	gl_Position = uTransform * vec4(FragPos, 1.0);
}