#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexture;
layout(location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uTransform;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
	TexCoords = aTexture;
	FragPos = vec3(uModel * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(uModel))) * aNormal;

	gl_Position = uTransform * vec4(FragPos, 1.0);
}