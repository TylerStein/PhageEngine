#version 400

//Material info in
uniform sampler2D fTexture;
uniform sampler2D fSpecularMap;
uniform vec3 fDiffuse;
uniform vec3 fAmbient;
uniform vec3 fSpecular;
uniform float fShininess;
uniform vec3 fEmit;

//Max 12 lights
//Light info in
uniform int lightCount;
uniform int lType[12];
uniform vec3 lPosition[12];
uniform vec3 lAngle[12];
uniform vec3 lColor[12];
uniform vec3 lPower[12];
uniform vec3 lAmbient[12];
uniform float lRange[12];
uniform float lCone[12];

//Camera location info
uniform vec3 cameraLocation;

//In from vertex shader
in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

//Get camera and model view matrices
in mat4 fCameraView;
in mat4 fModelView;
in mat4 fProjView;

//Final color out
out vec4 finalColor;

//Gamma const
const float screenGamma = 1.6;

void linearFalloff(int idx, float dist, out float percentOut){
	float dst = (lRange[idx] - dist) / lRange[idx];
	percentOut = clamp(dst, 0.0, 1.0);
}

void exponentFalloff(int idx, float dist, out float percentOut){
	float dst = clamp(1.0 - dist / lRange[idx], 0.0, 1.0); 
	dst *= dst;
	percentOut = clamp(dst, 0.0, 1.0);
}

//Applies value from 0-1 representing the specular map's power at this pixel
void SampleSpecularMap(out float percent){
	//Get the red value (any would work) from this UV sample
	percent = (texture(fSpecularMap, fTexCoord).r);
}

void light(int idx, vec3 pos, vec3 norm, out vec3 ambientOut, out vec3 diffuseOut, out vec3 specularOut){
	float rangeMult = 1.0;
	float angleMult = 1.0;
	vec3 lightDir = vec3(0.0);

	vec3 cPos = cameraLocation;
	vec3 tN = fNormal;

	//Point light
	if(lType[idx] == 0){
		//Light-model vectors
		vec3 lightVector = lPosition[idx] - fPosition;
		lightDir = normalize(lightVector);

		//Get the dot product of surface normal and light vectors
		angleMult = max(dot(lightDir, normalize(norm)), 0.0);

		//Falloff multiplier
		exponentFalloff(idx, length(lightVector), rangeMult);
	}
	//Spot light
	else if(lType[idx] == 1){
		//Light-model vectors
		vec3 lightVector = lPosition[idx] - fPosition;
		lightDir = normalize(lightVector);

		//Find out if the angle to surface is within the cone
		float toSurface = degrees(acos(dot(lightDir, normalize(lAngle[idx]))));
		if(toSurface > (lCone[idx] + 1.0)){
			//Outside of the cone, angle multiplier becomes 0
			angleMult = 0.0;
		}else{
			//Get the dot product of surface normal and light vectors
			angleMult = max(dot(lightDir, normalize(norm)), 0.0);

			//Falloff multiplier
			exponentFalloff(idx, length(lightVector), rangeMult);
		}

	}
	//Directional light
	else if(lType[idx] == 2){
		//Light-model vectors (angle-based)
		vec3 lightVector = lAngle[idx];
		vec3 lightDir = normalize(lightVector);

		//Get the dot product of surface normal and light vectors
		angleMult = max(dot(lightDir, normalize(norm)), 0.0);

		//Directional light has no range/falloff
		rangeMult = 1.0;
	}
	
	//Ambient
	ambientOut = lAmbient[idx] * fAmbient * rangeMult;

	//Diffuse
	diffuseOut = lColor[idx] * fDiffuse * angleMult * rangeMult;

	//Specular Highlights
	//Don't bother doing calculations if the object has no shine
	if(fShininess > 0.0){ 
		vec3 reflectVector = normalize(reflect(lightDir, norm));

		vec3 vertToEye = normalize(cameraLocation - fPosition);

		float specularFactor = dot(vertToEye, reflectVector);

		if(specularFactor < 0.0){
			specularFactor = pow(-specularFactor, fShininess);
			specularOut = lPower[idx] * fSpecular * specularFactor * rangeMult;
		}else{
			specularOut = vec3(0.0, 0.0, 0.0);
		}
	}else{
		specularOut = vec3(0.0, 0.0, 0.0);
	}
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

	//Apply specular map
	float textureSpecular = 1.0;
	SampleSpecularMap(textureSpecular);
	specularSum *= textureSpecular;
	
	//Average the scene ambience
	ambientSum /= lightCount;

	//Clamp the ambient value to be material's ambient at minimum
	vec3 finalAmb = clamp(ambientSum, fAmbient, vec3(1.0));

	vec4 texColor = texture(fTexture, fTexCoord);

	float fGamma = 1.0 / screenGamma;
	vec4 gamma = vec4(fGamma, fGamma, fGamma, 1.0);

	//finalColor = (vec4(ambientSum + diffuseSum, 1) * texColor) + vec4(specularSum, 1);
	finalColor = pow(vec4(finalAmb + diffuseSum, 1) * texColor + vec4(specularSum, 1), gamma);
}