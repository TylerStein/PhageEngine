#version 400
uniform sampler2D fTexture;

in vec2 fTexCoord;
in vec4 fColor;

out vec4 finalColor;

void main(){
	finalColor = texture(fTexture, fTexCoord) * fColor;
}