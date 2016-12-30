#version 400

in vec3 vPosition;
in vec3 vNormal;
in vec2 vCoordinate;
in vec3 vColor;

uniform mat4 cameraView;
uniform mat4 projectionView;
uniform mat4 modelView;

void main(){
	gl_Position = projectionView * cameraView * modelView * vec4(vPosition, 1);
}