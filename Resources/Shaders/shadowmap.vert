#version 400

in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;

uniform mat4 MVP;

out vec2 fTexCoord;

void main(){
	fTexCoord = vTexCoord;

	gl_Position = MVP * vec4(vPosition, 1.0);
}