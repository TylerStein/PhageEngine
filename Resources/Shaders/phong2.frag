#version 400

//Material info in
uniform sampler2D fTexture;
uniform vec3 fDiffuse;
uniform vec3 fAmbient;
uniform vec3 fSpecular;
uniform float fShininess;
uniform vec3 fEmit;

//Max 12 lights
//Light info in
uniform int lightCount;
uniform vec3 lPosition[12];
uniform vec3 lColor[12];
uniform vec3 lPower[12];
uniform vec3 lAmbient[12];
uniform float lAttenuation[12];
uniform float lRange[12];

//In from vertex shader
in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;
in vec3 fTangent;
in vec3 fBiTangent;

//Get camera and model view matrices
in mat4 fCameraView;
in mat4 fModelView;

//Final color out
out vec4 finalColor;

//Gamma const
const float screenGamma = 2.0;

void light(int idx, vec3 pos, vec3 norm, out vec3 ambientOut, out vec3 diffuseOut, out vec3 specularOut){
	vec3 n = normalize(norm);
	vec3 s = normalize(lPosition[idx] - pos);
	vec3 v = normalize(-pos);
	vec3 r = reflect(-s, n);

	ambientOut = lAmbient[idx] * fAmbient;

	float sDotN = max(dot(s, n), 0.0);

	diffuseOut = lColor[idx] * fDiffuse * sDotN;

	specularOut = lPower[idx] * fSpecular * pow(max(dot(r,v), 0.0), fShininess);
}

void main(){
	vec3 ambientSum = vec3(0.0);
	vec3 diffuseSum = vec3(0.0);
	vec3 specularSum = vec3(0.0);
	vec3 ambient, diffuse, specular;

	if(gl_FrontFacing){
		for(int i = 0; i < lightCount; ++i){
			light(i, fPosition, fNormal, ambient, diffuse, specular);
			ambientSum += ambient;
			diffuseSum += diffuse;
			specularSum += specular;
		}
	}else{
		for(int i = 0; i < lightCount; ++i){
			light(i, fPosition, -fNormal, ambient, diffuse, specular);
			ambientSum += ambient;
			diffuseSum += diffuse;
			specularSum += specular;
		}
	}
	
	ambientSum /= lightCount;

	vec4 texColor = texture(fTexture, fTexCoord);

	finalColor = vec4(ambientSum + diffuseSum, 1.0) * texColor + vec4(specularSum, 1.0);
}