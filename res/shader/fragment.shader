#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3) * vec4(Color, 1.0);

	vec3 ambient = light.ambient * uMaterial.ambient;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * uMaterial.diffuse);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = light.specular * (spec * uMaterial.specular);

	vec3 result = ambient + diffuse + specular;


	FragColor = vec4(result, 1.0);
	/*FragColor = vec4(1.0, 1.0, 1.0, 1.0);*/
}
