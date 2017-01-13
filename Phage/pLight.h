#pragma once
#include "glm/vec3.hpp"
#include "GL/glew.h"

//Light struct, keeps information together
struct Light {
	enum LightType{POINT, SPOT, DIRECTIONAL};
	LightType type;			//Light's type identifier

	float attenuation;		//Light's attenuation
	float range;			//Light's range of effect
	float cone;				//Light's cone radius (spot light)

	glm::vec3 position;		//Light's position in the world
	glm::vec3 color;		//Light's color
	glm::vec3 ambient;		//Light's ambient color
	glm::vec3 intensity;	//Light's intensity multiplier
	glm::vec3 eAngle;		//Light's direction (spot/directional light)
};

class pLight
{
public:
	//Point light constructor
	pLight(Light::LightType type, glm::vec3 position, GLfloat range = 25.0f, glm::vec3 color = glm::vec3(1.0f), glm::vec3 intensity = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f), GLfloat attenuation = 1.0f);
	
	//Directional light constructor
	pLight(Light::LightType type, glm::vec3 angle, glm::vec3 color = glm::vec3(1.0f), glm::vec3 intensity = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f), GLfloat attenuation = 1.0f);

	//Spot light constructor
	pLight(Light::LightType type, glm::vec3 position, glm::vec3 angle, glm::vec3 color = glm::vec3(1.0f), GLfloat range = 25.0f, GLfloat cone = 75.0f, glm::vec3 intensity = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f), GLfloat attenuation = 1.0f);

	~pLight();

	glm::vec3 getPosition();
	glm::vec3 getIntensity();
	glm::vec3 getAmbient();
	glm::vec3 getColor();
	float getAttenuation();
	float getRange();


	Light getLight();

	void translate(glm::vec3 trns);
	void setPosition(glm::vec3 pos);
	void setIntensity(glm::vec3 pow);
	void setAttenuation(GLfloat atten);
	void setAmbient(glm::vec3 ambient);
	void setRange(GLfloat rng);
	void setAngle(glm::vec3 angle);

private:
	Light lightInfo;
};

