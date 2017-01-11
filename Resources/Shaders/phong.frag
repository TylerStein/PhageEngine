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

void main(){
	//Fragment normal
	vec3 normal = fNormal;

	//Get tangent and bitangent
	//vec3 tangent = 

	//Find the texture surface color
	vec4 surfaceColor =  texture2D(fTexture, fTexCoord);

	if(lightCount > 0){
		//Prepare values to be mixed
		vec3 mixDiffuse = vec3(0.0);
		vec3 mixAmbient = vec3(0.0);
		vec3 mixSpecular = vec3(0.0);
		vec3 mixEmit = vec3(0.0);

		for(int i = 0; i < lightCount; ++i){
			//Light position in model view
			vec4 lightModelPos = vec4(lPosition[i], 1.0);

			//Light position in camera view
			vec4 lightCameraPos = fCameraView * fModelView * vec4(lPosition[i], 1.0);

			//Vector between fragment and light
			vec3 lightVector = lightModelPos.xyz - fPosition;

			//Light direction
			vec3 lightDir = normalize(lightVector);

			//Distance to light
			float distanceToLight = length(lightVector);

			//Angle between light and surface normal determines brightness
			float lambert = max(dot(lightDir, normal), 0.0);
			float specular = 0.0;

			//No light-affected properties if lambert is 0
			if(lambert > 0.0){
				//Direction to eye (should this be negative?)
				vec3 viewDir = normalize(-fPosition);

				//More efficient but less accurate "blinn-phong" calculation
				//vec3 halfDir = normalize(lightDir + viewDir);
				//float specAngle = max(dot(halfDir, normal), 0.0);
				//specular = pow(specAngle, fShininess);

				//Regular phong calculation (broken)
				vec3 reflectDir = reflect(-lightDir, normal);
				float specAngle = max(dot(reflectDir, viewDir), 0.0);
				specular = pow(specAngle, pow(fShininess, 0.2));
		

			}

			float distancePercent = (lRange[i] / distanceToLight) - 1.0;
			distancePercent = clamp(distancePercent, 0.0, 1.0);

			if(distancePercent <= 0.01){
				distancePercent = 0;
			}

			vec3 diffuseColor = fDiffuse * lColor[i] * distancePercent * lambert * lambert;
			mixDiffuse = (mixDiffuse + diffuseColor) * lAttenuation[i];

			vec3 specularColor = specular * mixDiffuse * fSpecular * distancePercent;
			mixSpecular = (mixSpecular + specularColor) * lAttenuation[i];

			vec3 ambientColor = fAmbient * lAmbient[i];
			mixAmbient = (mixAmbient + ambientColor) * lAttenuation[i];

			vec3 emitColor = vec3(0.0);
			mixEmit = (mixEmit + emitColor) * lAttenuation[i];
		}

		vec3 finalDiffuse = surfaceColor.xyz * mixDiffuse;
		vec3 finalAmbient = surfaceColor.xyz * mixAmbient;
		vec3 finalSpecular = mixSpecular;
		vec3 finalEmit = mixEmit;

		vec3 linearColor = finalAmbient + finalEmit + finalSpecular + finalDiffuse;

		//Apply gamma correction
		vec3 gammaCorrectColor = pow(linearColor, vec3(2.2 / screenGamma));

		//Set the final color
		finalColor = vec4(gammaCorrectColor, surfaceColor.a);
	}
	else{
		finalColor = (vec4(fDiffuse, 1.0) * surfaceColor);
	}
}
