#version 400

struct VertData {
	vec2 texCoord;
	vec4 color;
	vec3 normal;
	vec3 vertex;
};

in vec4 fColor;

//Output the final pixel color
out vec4 finalColor;

void main(){
	finalColor = fColor;
}