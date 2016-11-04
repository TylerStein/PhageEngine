#version 400
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vNormal;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform vec3 lightsource;
uniform vec3 lightpower;

out vec4 fColor;
out vec2 fTexCoord;
out vec3 fNormal;

out vec3 fVert;
out vec3 fLightSource;
out vec3 fLightPower;

out mat4 fModel;

void main(){
	fColor = vColor;
	fTexCoord = vTexCoord;
	fNormal = vNormal;

	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vPosition, 1);

	fModel = model_matrix;

	fLightPower = lightpower;
	fLightSource = lightsource;
	fVert = vPosition;
}