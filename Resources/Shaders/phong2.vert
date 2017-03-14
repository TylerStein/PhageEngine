#version 400

//Vertex in
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;

//View matrix in
uniform mat4 cameraView;
uniform mat4 modelView;
uniform mat4 projView;
uniform mat3 normalView;

//Light info in
uniform int vLightCount;
uniform vec3 vLightPos[12];

//Out to fragment shader
out vec2 fTexCoord;
out vec3 fNormal;
out vec3 fTangent;

out vec3 fToCamera;

flat out int fLightCount;
out vec3 fToLight[12];
out float fLightDistance[12];

void main(){
	vec4 position_world = modelView * vec4(vPosition, 1.0);

	mat4 MVM = cameraView * modelView;
	
	vec4 position_camera = MVM * vec4(vPosition, 1.0);

	fTexCoord = vTexCoord;
	fNormal = (MVM * vec4(vNormal, 0.0)).xyz;

	fLightCount = vLightCount;
	for(int i = 0; i < vLightCount; ++i){
		vec3 lightPos_eyespace = (cameraView * vec4(vLightPos[i], 1.0)).xyz;
		fToLight[i] = lightPos_eyespace - position_camera.xyz;
		fLightDistance[i] = length(fToLight[i]);
	}

	fToCamera = position_camera.xyz;
	fTangent = vTangent;
	fTexCoord = vTexCoord;

	gl_Position = projView * position_camera; 
}