#version 330 core

const uint MAX_BONES = 100u;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexture;
layout(location = 3) in vec3 aTangents;
layout(location = 4) in vec3 aBiTangents;
layout(location = 5) in ivec4 aBoneIDs;
layout(location = 6) in vec4 aWeights;

uniform mat4 uModel;
uniform mat4 uTransform;
uniform mat4 uBones[MAX_BONES];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{	
	mat4 BoneTransform = (uBones[aBoneIDs[0]] * aWeights[0]);
	BoneTransform += (uBones[aBoneIDs[1]] * aWeights[1]);
	BoneTransform += (uBones[aBoneIDs[2]] * aWeights[2]);
	BoneTransform += (uBones[aBoneIDs[3]] * aWeights[3]);
	
	vec4 posL = BoneTransform * vec4(aPos, 1);
	vec4 normalL = BoneTransform * vec4(aNormal, 0);
	
	gl_Position = uTransform * uModel * posL;

	TexCoords = aTexture;

	Normal = (uModel * normalL).xyz;
	FragPos = (uModel * posL).xyz;


}