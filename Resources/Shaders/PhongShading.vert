#version 400
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vNormal;

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
uniform View vView;
uniform int vFlags;

//Outputs to Fragment Shader
out View fView;
out VertData fVert;
flat out int fFlags;

void main(){
	//Pass flags on to the fragment shader
	fFlags = vFlags;

	//Pass vertex and matrix info on to the fragment shader
	fVert.texCoord = vTexCoord;
	fVert.color = vColor;
	fVert.normal = vNormal;
	fVert.vertex = vPosition;

	fView.camera = vView.camera;
	fView.projection = vView.projection;
	fView.model = vView.model;

	//Calculate the position from matrices and vertex position
	vec4 finalPos =  vView.projection * vView.camera * vView.model * vec4(vPosition, 1);

	//Set the vertex position to this final position
	gl_Position = finalPos;
}