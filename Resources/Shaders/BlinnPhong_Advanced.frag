//GLSL Fragment Shader
#version 400

//Material info : Basic
uniform vec3	fDiffuse;
uniform vec3	fAmbient;
uniform vec3	fEmit;
uniform vec3	fSpecular;
uniform float	fShininess;

//Material info : Textures
uniform sampler2D fTexture;
uniform sampler2D fSpecularMap;
uniform sampler2D fNormalMap;

//Light info : Type
uniform int		lType[12]; //0 = Point, 1 = Spot, 2 = Directional

//Light info : World
uniform float	lRange[12];
uniform float	lCone[12];
uniform vec3	lAngle[12];

//Light info : Effect
uniform vec3	lColor[12];
uniform vec3	lPower[12];
uniform vec3	lAmbient[12];

//Camera world position
uniform vec3 cameraLocation;

//In from vertex shader
in vec2 fTexCoord;
in vec3 fTangent;
in vec3 fNormal_world;
in vec3 fPos_world;
in vec3 fPos_tangent;

flat in int fLightCount;
in vec3 fLightPosition[12];
in vec3 lightVector_tangent[12];
in vec3 lightVector[12];

in mat3 fTangentMatrix;
//Output to screen
out vec4 finalColor;

const float screenGamma = 1.6;

void SampleNormalMap(out vec3 outNormalValue){
	outNormalValue = texture2D(fNormalMap, fTexCoord).rgb * 2.0 - 1.0;
}

void SampleSpecularMap(out float outSpecValue){
	outSpecValue = texture2D(fSpecularMap, fTexCoord).r;
}

//Exponential attenuation falloff (good)
void ExponentFalloff(float range, float dist, out float attenuation){
	if(dist > range){
		attenuation = 0.0;
	}else{
		attenuation = clamp(pow(1.0 - dist / range, 2.0), 0.0, 1.0);
	}
}

//Linear attenuation falloff (bad)
void LinearFalloff(float range, float dist, out float attenuation){
	attenuation = clamp((range - dist) / range, 0.0, 1.0);
}

//Expensive phong reflection model
void Specular_Phong(int idx, vec3 lightDir, vec3 surfaceNormal, out float specularValue){
	vec3 reflectVector = normalize(reflect(lightDir, surfaceNormal)); //Get the reflection vector of the light direction and surface normal
	vec3 vertToEye = normalize(cameraLocation - fPos_world); //Get the vector from the surface to the camera
	float specularFactor = dot(vertToEye, reflectVector); //Get the scalar difference between the reflection and to-camera vectors

	//Check if there is any specular reflection happening
	if(specularFactor < 0.0){
		float textureSpecular = 1.0; //Initialize texture specular in case there's no specular texture
		SampleSpecularMap(textureSpecular); //Sample the texture map
		specularValue = pow(-specularFactor, fShininess); //Scale the surface shine to the power of the material shininess
		specularValue = specularValue * textureSpecular;
	}else{
		specularValue = 0.0; //No specular shine
	}
}

//Faster blinn-phong approximation reflection model
void Specular_BlinnPhong(int idx, vec3 lightDir, vec3 surfaceNormal, out float specularValue){
	vec3 camVector = normalize(cameraLocation - fPos_world);

	vec3 halfVector = normalize(lightDir + camVector);

	float specularFactor = max(0.0, dot(surfaceNormal, halfVector));
	float specularPower = pow(specularFactor, fShininess);

	float specularTex = 1.0;
	SampleSpecularMap(specularTex);

	specularValue = specularPower * (specularTex * specularTex);
}

//Calculate the angle of incidence and attenuation falloff for the light at idx
void Light(int idx, vec3 surfaceNormal, out vec3 diffuseOut, out vec3 specularOut, out vec3 ambientOut){
	//Initialize values in case uniforms are missing
	float attenuation = 1.0;
	float lambert = 1.0;
	vec3 lightDir = vec3(0.0);


	vec3 lightPos_tangent = fTangentMatrix * fLightPosition[idx];
	vec3 lightAngle_tangent = fTangentMatrix * lAngle[idx];
	vec3 lightVector_tangent = lightPos_tangent - fPos_tangent;
	vec3 lightVector_world = fLightPosition[idx] - fPos_world;

	int type = lType[idx];

	if(type == 0){
		//Point light
		 //Get world space vector from light to surface
		lightDir = normalize(lightVector[idx]); //Create a normalized direction from the light-to-surface vector
		lambert = max(dot(lightDir, normalize(surfaceNormal)), 0.0); //Get the dot product of the surface normal and incoming light vector
		ExponentFalloff(lRange[idx], length(lightVector[idx]), attenuation); //Calculate the attenuation falloff for this light
	}else if(type == 1){
		//Spot light
		lightDir = normalize(lightVector[idx]); //Create a normalized direction from the light-to-surface vector
		float toSurface = degrees(acos(dot(lightDir, normalize(lAngle[idx])))); //Find this surface's distance from the cone center
		float edgeFade = 1.0f;
		ExponentFalloff(lCone[idx], toSurface - (lCone[idx] * 0.12f), edgeFade);
		//LinearFalloff(lCone[idx], toSurface, edgeFade);
		if(toSurface > (lCone[idx] + 1.0)){
			lambert = 0.0; //Outside of the cone, zero light
			attenuation = 0.0;
		}else{
			lambert = max(dot(lightDir, normalize(surfaceNormal)), 0.0); //Get the dot product of the surface normal and incoming light vector
			ExponentFalloff(lRange[idx], length(lightVector_world), attenuation);
		}
		lambert = clamp(lambert * edgeFade, 0.0, lambert);
	}else if(type == 2){
		//Directional/infinite light
		lightDir = normalize(lAngle[idx]); //Create a normalized direction from the light-to-surface vector
		lambert = max(dot(lightDir, normalize(surfaceNormal)), 0.0); //Get the dot product of the surface normal and incoming light vector
		attenuation = 1.0; //Infinite light has constant attenuation (no falloff)
	}else{
		//Invalid light type (unlit)
		attenuation = 1.0;
		lambert = 1.0;
		diffuseOut = vec3(1.0);
		specularOut = vec3(0.0);
		ambientOut = vec3(1.0);
		return;
	}

	//Multiply light and surface's desired ambient values
	ambientOut = lAmbient[idx] * fAmbient;

	//Affect diffuse color
	diffuseOut = lColor[idx] * fDiffuse * attenuation * lambert;



	//Calculate specular highlights (if the material has any shine)
	if(fShininess > 0.0){
		float specularFactor = 0.0;
		Specular_BlinnPhong(idx, lightDir, surfaceNormal, specularFactor);
		specularOut = ((lPower[idx] * fSpecular * specularFactor) * attenuation);
	}else{
		specularOut = vec3(0.0); //No shine, no specular!
	}
}

void main(){
	//Values to track accumulated light-affected components
	vec3 ambientSum = vec3(0.0);
	vec3 diffuseSum = vec3(0.0);
	vec3 specularSum = vec3(0.0);

	//Temporary values for plugging in to functions
	vec3 amb, diff, spec;

	vec3 normalMap;
	normalMap = (2.0 * texture2D(fNormalMap, fTexCoord).rgb - 1.0);
	//normalMap.xy = (2.0 * texture2D(fNormalMap, fTexCoord).rg - 1.0);
	//normalMap.z = sqrt(1.0 - dot(normalMap.xy, normalMap.xy));

	vec3 adjustedNormal = fNormal_world;

	//Check if the surface is front facing?
	if(gl_FrontFacing){
		for(int i = 0; i < fLightCount; ++i){
			Light(i, adjustedNormal, diff, spec, amb); //Run the light calculations function
			//Add the calculated components
			diffuseSum += diff;
			specularSum += spec;
			ambientSum += amb;
		}
	}else{
		for(int i = 0; i < fLightCount; ++i){
			Light(i, -adjustedNormal, diff, spec, amb); //Run the light calculations function (inversed normal for back-facing?)
			//Add the calculated components
			diffuseSum += diff;
			specularSum += spec;
			ambientSum += amb;
		}
	}

	//Average out the final ambient lighting
	ambientSum /= fLightCount;

	//Clamp the ambient value between the material ambient and 1.0
	ambientSum = clamp(ambientSum, fAmbient, vec3(1.0));

	//Sample the diffuse texture color
	vec4 texColor = texture(fTexture, fTexCoord);

	//Calculate gamma correction
	vec4 gamma = vec4(1.0 / screenGamma);
	gamma.w = 1.0;

	finalColor = pow(vec4(ambientSum + diffuseSum, 1.0) * texColor + vec4(specularSum, 1.0) + vec4(fEmit, 1.0), gamma);
}
