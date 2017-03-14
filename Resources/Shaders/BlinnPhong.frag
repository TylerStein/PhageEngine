#version 400

//Material Info : Colors
uniform vec3 fDiffuse;
uniform vec3 fAmbient;
uniform vec3 fSpecular;
uniform float fShininess;

//Material Info : Textures
uniform sampler2D fDiffuseMap;
uniform sampler2D fSpecularMap;
uniform sampler2D fNormalMap;

//Light Info
uniform vec3 lAngle[12];
uniform vec3 lColor[12];
uniform vec3 lPower[12];
uniform vec3 lAmbient[12];
uniform float lRange[12];
uniform float lCone[12];
uniform int lType[12];

//Camera world locatio
uniform vec3 fCameraPos;

//In from Vertex shader
in vec2 fTexCoord;
in vec3 fPosition_world;
flat in int fLightCount;
in vec3 lightVector_tangent[12];
in float lightDist_world[12];
in mat3 TBNMatrix;

//Fragment color out
out vec4 finalColor;

const float screenGamma = 2.0;

//Exponential attenuation falloff
void CalculateFalloff(float range, float dist, out float attenuation){
	if(dist > range){
		attenuation = 0.0;
	}else{
		attenuation = clamp(pow(1.0 - dist / range, 2.0), 0.0, 1.0);
	}
}

void CalculateSpecular_BlinnPhong(int idx, vec3 _surfaceNormal, vec3 _lightVector, out vec3 _specular){
	vec3 cameraVector = normalize(fCameraPos - fPosition_world);
	vec3 halfVector = normalize(_lightVector + cameraVector);
	float nDotH = max(0.0, dot(_surfaceNormal, halfVector));
	float specularFactor = pow(nDotH, fShininess);
	_specular = fSpecular * lPower[idx] * specularFactor;
}


void CalculateLight_Point(int idx, vec3 _surfaceNormal, out vec3 _diffuse, out vec3 _specular, out vec3 _ambient){
	//Calculate light attenuation
	float attenuation = 1.0;
	CalculateFalloff(lRange[idx], lightDist_world[idx], attenuation);

	vec3 lightVec_norm = normalize(lightVector_tangent[idx]);
	float nDotL = max(0.0, dot(_surfaceNormal, lightVec_norm));

	_diffuse = fDiffuse * lColor[idx] * nDotL * attenuation;
	_ambient = fAmbient * lAmbient[idx];

	if(nDotL != 0.0){
		vec3 tmpSpec = vec3(0.0);
		CalculateSpecular_BlinnPhong(idx, _surfaceNormal, lightVec_norm, tmpSpec);
		_specular = tmpSpec * attenuation;
	}else{
		_specular = vec3(0.0);
	}
}

void main(){
	//Sample normal, specular, and diffuse texture maps
	vec3 norm_map = texture2D(fNormalMap, fTexCoord).rgb * 2.0 - 1.0;
	float spec_map = texture2D(fSpecularMap, fTexCoord).r;
	vec4 diff_map = texture2D(fDiffuseMap, fTexCoord);

	vec3 surfaceNormal = norm_map;

	//Create empty variables for storing cumulative light effects
	vec3 diffuseSum = vec3(0.0);
	vec3 specularSum = vec3(0.0);
	vec3 ambientSum = vec3(0.0);

	//Iterate through each light
	for(int i = 0; i < fLightCount; ++i){
		vec3 tmpDiffuse;
		vec3 tmpSpecular;
		vec3 tmpAmbient;

		if(lType[i] == 0){
		//Point light
		CalculateLight_Point(i, surfaceNormal, tmpDiffuse, tmpSpecular, tmpAmbient);

		}else if(lType[i] == 1){
		//Spot light


		}else if(lType[i] == 2){
		//Directional/infinite light

		}else{
		//Invalid light
			
		}

		//Apply the surface colors from this light iteration
		diffuseSum += tmpDiffuse;
		ambientSum += tmpAmbient;
		specularSum += tmpSpecular;
	}

	//Average out ambient sum, clamp minimum to material ambient
	ambientSum = clamp(ambientSum, fAmbient, vec3(1.0));

	//Calculate final color pre-gamma correction		
	vec4 fColor = vec4(ambientSum + diffuseSum, 1.0) * diff_map + vec4(specularSum, 1.0);

	//Calculate gamma correction
	vec4 gamma = vec4(1.0 / screenGamma);
	gamma.w = 1.0;

	//Pass final color
	finalColor = pow(fColor, gamma);
}