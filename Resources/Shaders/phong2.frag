#version 400

//Material info in
uniform sampler2D fTexture;
uniform sampler2D fNormalMap;
uniform sampler2D fSpecularMap;
uniform vec3 fDiffuse;
uniform vec3 fAmbient;
uniform vec3 fSpecular;
uniform float fShininess;

//Max 12 lights
//Light info in
uniform int lType[12];
//uniform vec3 lPosition[12];
uniform vec3 lAngle[12];
uniform vec3 lColor[12];
uniform vec3 lPower[12];
uniform vec3 lAmbient[12];
uniform float lRange[12];
uniform float lCone[12];

//In from vertex shader
in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fTangent;

in vec3 fToCamera;

flat in int fLightCount;
in vec3 fToLight[12];
in float fLightDistance[12];

out vec4 finalColor;

void main(){
	vec3 unitNormal = normalize(fNormal);
	vec3 unitToCamera = normalize(fToCamera);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalAmbient = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for(int i = 0; i < fLightCount; ++i){
		float attenuation = clamp(pow(1.0 - fLightDistance[i] / lRange[i], 2.0), 0.0, 1.0);
	
		vec3 unitToLight = normalize(fToLight[i]);
		float nDotl = dot(unitNormal, unitToLight);

		float brightness = max(nDotl, 0.0);

		vec3 lightDir = -unitToLight;
		vec3 reflectedLightDir = reflect(lightDir, unitNormal);

		float specularFactor = dot(reflectedLightDir, unitToCamera);
		specularFactor = max(specularFactor, 0.0);

		float dampenedFactor = pow(specularFactor, fShininess);

		totalDiffuse = totalDiffuse + (lColor[i] * brightness * attenuation);
		totalAmbient = totalAmbient + (lAmbient[i]);
		totalSpecular = totalSpecular + (lPower[i] * fSpecular[i] * specularFactor * dampenedFactor);
	}

	totalDiffuse = totalDiffuse / fLightCount;
	totalAmbient = totalAmbient / fLightCount;
	totalAmbient = clamp(totalAmbient, fAmbient, vec3(0.0));
	totalSpecular = totalSpecular / fLightCount;
	
	vec3 diffuseTex = texture(fTexture, fTexCoord).xyz;
	vec3 normalTex = texture(fNormalMap, fTexCoord).xyz;
	
	finalColor = vec4(totalAmbient + totalDiffuse * diffuseTex, 1.0);

	//finalColor = vec4(((ambient + diffuse) * diffuseTex) + specular, 1.0); 
}