//GLSL Vertex Shader
#version 400

//Incoming local space vertex values
in vec3 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec2 vTexCoord;

//Incoming matrices
uniform mat4 cameraView;
uniform mat4 projView;
uniform mat4 modelView; //Model transformation matrix (local to world)
uniform mat3 normalView; //Normal matrix for non-uniform scaled normals fix

//Out to fragment shader
out vec2 fTexCoord;
out vec3 fNormal_world;
out vec3 fPos_world;
out vec3 fTangent;

out mat3 fTangentMatrix;

void main(){
	fTexCoord = vTexCoord;
	fTangent = vTangent;

	//Get the world-relative face normal
	fNormal_world = normalize(normalView * vNormal);
	fPos_world = (modelView * vec4(vPosition, 1.0)).xyz;

	vec3 fTangent_world = normalize(normalView * vTangent);
	vec3 fBiTangent_world = cross(fNormal_world, fTangent_world);

	fTangentMatrix = mat3(fTangent_world, fBiTangent_world, fNormal_world);

	gl_Position = projView * cameraView * modelView * vec4(vPosition, 1.0);
}