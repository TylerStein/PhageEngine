#version 400

uniform vec3 diffuseColor;

out vec4 finalColor;


void main(){
	finalColor = vec4(diffuseColor, 1.0);
}