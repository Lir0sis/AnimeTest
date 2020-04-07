#version 330 core

//uniform mat4 locals;
//uniform mat4 globals;
//uniform mat4 view;
//uniform mat4 proj;

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
//layout(location = 2) in vec2 aTexCoord;

out vec3 Color;
//out vec2 TexCoord;

void main()
{
	Color = color;
	gl_Position = vec4(position, 0.0f, 1.0f);
	//TexCoord = aTexCoord;
	//gl_Position = proj * view * globals * locals * vec4(position, 1.0);
}