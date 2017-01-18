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

void falloff(int idx, float dist, out float percentOut){
	float dst = (lRange[idx] - dist) / lRange[idx];
	percentOut = clamp(dst, 0, 1);
}

void light(int idx, vec3 pos, vec3 norm, out vec3 ambientOut, out vec3 diffuseOut, out vec3 specularOut){
	vec3 n = normalize(norm);
	vec3 diff = lPosition[idx] - pos;
	float dist = length(diff);
	vec3 s = normalize(diff);
	vec3 v = normalize(-pos);
	vec3 r = reflect(-s, n);

	float rangeMult = 1;
	falloff(idx, dist, rangeMult);

	ambientOut = lAmbient[idx] * fAmbient * rangeMult;

	float sDotN = max(dot(s, n), 0.0);

	diffuseOut = lColor[idx] * fDiffuse * sDotN * rangeMult;

	specularOut = lPower[idx] * fSpecular * pow(max(dot(r,v), 0.0), fShininess) * rangeMult;
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

	//Clamp the ambient value to be material's ambient at minimum
	float aX = clamp(ambientSum.x, fAmbient.x, 1.0);
	float aY = clamp(ambientSum.y, fAmbient.y, 1.0);
	float aZ = clamp(ambientSum.z, fAmbient.z, 1.0);

	vec3 finalAmb = vec3(aX, aY, aZ);

	vec4 texColor = texture(fTexture, fTexCoord);

	finalColor = (vec4(finalAmb + diffuseSum, 1.0) + vec4(specularSum, 1.0)) * texColor;
}