#version 400
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec4 fColor;
out vec2 fTexCoord;

void main(){
	fColor = vColor;
	fTexCoord = vTexCoord;
	gl_Position = projection_matrix * view_matrix * model_matrix * vPosition;
}