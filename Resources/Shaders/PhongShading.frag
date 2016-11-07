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

//Output the final pixel color
out vec4 finalColor;

void main(){
	vec3 normal = normalize(transpose(inverse(mat3(fView.model))) * fVert.normal);

	//Surface position (vertex * modelMatrix)
	vec3 surfacePos = vec3(fView.model * vec4(fVert.vertex, 1));
	//Surface color (diffuse texture)
	vec4 surfaceColor = texture(fTextures.diffuse, fVert.texCoord);
	//Vector pointing from surface to light
	vec3 surfaceToLight = normalize(fLight.position - surfacePos);

	//Vector pointing from surface to camera
	vec3 surfaceToCamera = normalize(vec3(0, 0, 0) - surfacePos);

	//Get ambient color
	vec3 ambient = fLight.ambient * surfaceColor.rgb * fLight.power;

	//Get diffuse color
	float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
	vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * fLight.power;

	//Get specular color
	float specularCoefficient = 0.0;
	if(diffuseCoefficient > 0.0){
		specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), fMaterial.shininess);
	}
	vec3 specular = specularCoefficient * fMaterial.specular * fLight.power;

	//Attenuation
	float distanceToLight = length(fLight.position - surfacePos);
	float attenuation = 1.0 / (1.0 + fLight.attenuation * pow(distanceToLight, 2));

	//Linear color (before gamma correction)
	vec3 linearColor = ambient + attenuation * (diffuse + specular);

	//Final color (after gamma correction)
	vec3 gamma = vec3(1.0 / 2.2);
	finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);

	/*
	vec3 V = fVertex_modelView;
	vec3 N = fNormal_cameraView;
	vec3 L = normalize(fLight.position - V);
	vec3 E = normalize(-V);
	vec3 R = normalize(-reflect(L, N));

	vec4 iAmb = vec4(fMaterial.ambient, 1) * texture(fTextures.diffuse, fVert.texCoord);
	vec4 iDiff = vec4(fMaterial.diffuse * max(dot(N, L), 0.0), 1);
	vec4 iSpec = vec4(fMaterial.specular * pow(max(dot(R,E), 0.0), 0.3 * fMaterial.shininess), 1);
	iSpec = clamp(iSpec, 0.0, 1.0);

	finalColor = vec4(fLight.color * fLight.power, 1) + iAmb + iDiff + iSpec;
	*/
	//Material Info		1
	//Diffuse texture	2
	//Specular texture	4
	//Bump texture		8
	//Phong Shading		16
}