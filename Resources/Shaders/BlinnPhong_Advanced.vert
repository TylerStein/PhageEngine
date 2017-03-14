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

//Light position in
uniform int	lightCount; //Max 12 lights
uniform vec3 lPosition[12];

//Out to fragment shader
out vec2 fTexCoord;
out vec3 fNormal_world;
out vec3 fNormal_tangent;
out vec3 fPos_world;
out vec3 fPos_tangent;
out vec3 fTangent;

flat out int fLightCount;
out vec3 lightVector_tangent[12];
out vec3 fLightPosition[12];

out mat3 fTangentMatrix;

void main(){
	fTexCoord = vTexCoord;
	fTangent = vTangent;

	//Get the world-relative face normal
	fNormal_world = normalize(normalView * vNormal);
	fPos_world = (modelView * vec4(vPosition, 1.0)).xyz;

	//Create the bitangent
	vec3 fBiTangent = cross(vNormal, vTangent);
	

	mat3 TBNMatrix = mat3(vTangent, fBiTangent, vNormal);
	fLightCount = lightCount;
	for(int i = 0; i < lightCount; ++i){
		fLightPosition[i] = lPosition[i];
		vec3 lightVector = lPosition[i] - fPos_world;
		lightVector_tangent[i] = lightVector * TBNMatrix;
	}


	//Create tangent-space vectors
	fPos_tangent = fTangentMatrix * vPosition;

	gl_Position = projView * cameraView * modelView * vec4(vPosition, 1.0);
}