#version 400

in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;

uniform mat4 MVPMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

uniform int lightCount;
uniform vec3 lPosition[12];

out vec2 fTexCoord;
out vec3 fPosition_world;
flat out int fLightCount;
out vec3 lightVector_tangent[12];
out float lightDist_world[12];
out mat3 TBNMatrix;

void main(){
	//Pass texture coordinates
	fTexCoord = vTexCoord;

	//Pass light count
	fLightCount = lightCount;

	//Calculate and pass world-space surface position
	fPosition_world = (modelMatrix * vec4(vPosition, 1.0)).xyz;

	//Calculate BiTanent and tangent-space matrix
	vec3 biNormal = cross(normalMatrix * vNormal, vTangent);
	TBNMatrix = mat3(vTangent, biNormal, vNormal);

	//Calculate tangent-space light vectors
	for(int i = 0; i < lightCount; ++i){
		lightVector_tangent[i] = lPosition[i] - fPosition_world;
		lightDist_world[i] = length(lightVector_tangent[i]);
		lightVector_tangent[i] *= TBNMatrix;
	}

	gl_Position = MVPMatrix * vec4(vPosition, 1.0);
}