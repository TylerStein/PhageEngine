#version 400

struct Material{
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
};

struct Textures{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D bump;
};

struct Light{
	vec3 position;
	vec3 color;
	vec3 power;
	float attenuation;
	float ambient;
};

struct View {
	mat4 camera;
	mat4 projection;
	mat4 model;
};

struct VertData {
	vec2 texCoord;
	vec4 color;
	vec3 normal;
	vec3 vertex;
};

//Uniform inputs
uniform Material fMaterial;
uniform Textures fTextures;
uniform Light fLight;

//Inputs from the Vertex Shader
in View fView;
in VertData fVert;
flat in int fFlags;
in vec3 fMovedVertex;

//Output the final pixel color
out vec4 finalColor;

void main(){


	mat3 normalMatrix = transpose(inverse(mat3(fView.model)));
	vec3 normal = normalize(normalMatrix * fVert.normal);

	//vec3 fragPosition = vec3(fView.model * vec4(fVert.vertex, 1));

	vec3 surfaceToLight = fLight.position - fVert.vertex;
	
	float brightness = dot(normal, surfaceToLight) / length(surfaceToLight) * length(normal);
	brightness = clamp(brightness, 0, 1);

	vec4 surfaceColor = texture(fTextures.diffuse, fVert.texCoord) * vec4(fMaterial.diffuse, 1);
	vec4 finalAmbient = vec4(fMaterial.ambient, 1.0);

	//finalColor = finalAmbient + surfaceColor * fVert.color;
	finalColor = finalAmbient + vec4(brightness * fLight.power * surfaceColor.rgb, surfaceColor.a);

	//Material Info		1
	//Diffuse texture	2
	//Specular texture	4
	//Bump texture		8
	//Phong Shading		16
}