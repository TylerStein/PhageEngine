#version 400
uniform sampler2D fTexture;

uniform struct Light {
	vec3 position;
	vec3 intensities;
} light;

in vec2 fTexCoord;
in vec4 fColor;
in vec3 fNormal;
in vec3 fVert;
in mat4 model;

out vec4 finalColor;

void main(){
	finalColor = texture(fTexture, fTexCoord) * fColor;
}