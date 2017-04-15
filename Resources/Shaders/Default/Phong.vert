#version 400

//Vertex in
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

in ivec4 BoneIDs;
in vec4 Weights;

//View matrix in
uniform mat4 cameraView;
uniform mat4 modelView;
uniform mat4 projView;
uniform mat3 normalView;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

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
	mat4 BoneTransform = bones[BoneIDs[0]] * Weights[0];
    BoneTransform += bones[BoneIDs[1]] * Weights[1];
    BoneTransform += bones[BoneIDs[2]] * Weights[2];
    BoneTransform += bones[BoneIDs[3]] * Weights[3];

	vec4 bNormal = BoneTransform * vec4(vNormal, 0.0);

	fNormal = normalize(normalView * bNormal.xyz);

	vec4 bPosition = BoneTransform * vec4(vPosition, 1.0);

	fPosition = vec3(modelView * bPosition);

	fModelView = modelView;
	fCameraView = cameraView;
	fProjView = projView;

	fTexCoord = vTexCoord;

	gl_Position =  projView * cameraView * modelView * bPosition;
}