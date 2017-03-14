#version 400

uniform samplerCube fCubeMapTexture;

in vec3 fTexCoord;

out vec4 finalColor;

void main(){
	finalColor = texture(fCubeMapTexture, fTexCoord);
}