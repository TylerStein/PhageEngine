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

uniform View vView;

out vec4 fColor;

void main(){
	fColor = vColor;

	//Set the vertex position to this final position
	gl_Position = vView.projection * vView.camera * vView.model * vec4(vPosition, 1);
}