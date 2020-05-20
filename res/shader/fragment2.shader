#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular2;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material uMaterial;
uniform vec3 viewPos;
uniform Light light;

out vec4 FragColor;

void main()
{
	// ambient
	vec3 ambient = light.ambient * texture(uMaterial.texture_diffuse1, TexCoords).rgb;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(uMaterial.texture_diffuse1, TexCoords).rgb;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = light.specular * spec * texture(uMaterial.texture_specular2, TexCoords).rgb;

	// result
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
