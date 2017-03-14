#version 400

uniform sampler2D shadowMap;

in vec2 fTexCoord;

out vec4 fragColor;

void main(){
	float depth = texture(shadowMap, fTexCoord).x;
	depth = 1.0 - (1.0 - depth) * 25.0;
	fragCOlor = vec4(depth);
}