#version 400

//Vertex in
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

//View matrix in
uniform mat4 cameraView;
uniform mat4 modelView;
uniform mat4 projView;
uniform mat3 normalView;

//View space position out
out mat4 fCameraView;
out mat4 fModelView;
out mat4 fProjView;

//Vertex position and normal out
out vec3 fPosition;
out vec3 fNormal;

//Texture coordinate out
out vec2 fTexCoord;

void main(){
	fNormal = normalize(normalView * vNormal);

	fPosition = vec3(modelView * vec4(vPosition, 1));

	fModelView = modelView;
	fCameraView = cameraView;
	fProjView = projView;

	fTexCoord = vTexCoord;

	gl_Position =  projView * cameraView * modelView * vec4(vPosition, 1);
}