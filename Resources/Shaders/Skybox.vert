#version 400

in vec3 vPosition;

uniform mat4 transformMatrix;

out vec3 fTexCoord;

void main(){
	vec4 worldPos = transformMatrix * vec4(vPosition, 1.0);
	fTexCoord = vPosition;
	gl_Position = worldPos.xyww;
}