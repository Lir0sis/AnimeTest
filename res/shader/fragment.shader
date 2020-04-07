#version 330 core

in vec3 Color;
//in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

out vec4 o_Color;

void main()
{
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3) * vec4(Color, 1.0);
	o_Color = vec4(Color, 1.0);
}
