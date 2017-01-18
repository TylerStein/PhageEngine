#version 400

//Vertex in
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

//View matrix in
uniform mat4 cameraView;
uniform mat4 modelView;
uniform mat4 projView;
uniform mat3 normalView;

//Camera position out
out mat4 fCameraView;
out mat4 fModelView;

//Vertex position and normal out
out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;
out vec3 fTangent;
out vec3 fBiTangent;

void main(){
	fNormal = normalize(normalView * vNormal);

	fPosition = vec3(modelView * vec4(vPosition, 1));

	fTexCoord = vTexCoord;
	fTangent = vTangent;
	fBiTangent = vBiTangent;


	gl_Position =  projView * cameraView * modelView * vec4(vPosition, 1);
}