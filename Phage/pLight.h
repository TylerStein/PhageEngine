#pragma once
#include "glm/vec3.hpp"
#include "GL/glew.h"

//Light struct, keeps information together
struct Light {
	enum LightType{POINT, SPOT, DIRECTIONAL};
	LightType type;			//Light's type identifier

	float range;			//Light's range of effect
	float cone;				//Light's cone radius (spot light)

	glm::vec3 position;		//Light's position in the world
	glm::vec3 color;		//Light's color
	glm::vec3 ambient;		//Light's ambient color
	glm::vec3 intensity;	//Light's specular effect
	glm::vec3 eAngle;		//Light's direction (spot/directional light)
};

class pLight
{
public:
	//Point light constructor
	pLight(Light::LightType type, glm::vec3 position, GLfloat range = 25.0f, glm::vec3 color = glm::vec3(1.0f), glm::vec3 intensity = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f));
	
	//Directional light constructor
	pLight(Light::LightType type, glm::vec3 angle, glm::vec3 color = glm::vec3(1.0f), glm::vec3 intensity = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f));

	//Spot light constructor
	pLight(Light::LightType type, glm::vec3 position, glm::vec3 angle, glm::vec3 color = glm::vec3(1.0f), GLfloat range = 25.0f, GLfloat cone = 75.0f, glm::vec3 intensity = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f));

	~pLight();

	glm::vec3 getPosition();
	glm::vec3 getIntensity();
	glm::vec3 getAmbient();
	glm::vec3 getColor();
	glm::vec3 getAngle();
	float getCone();
	int getType();
	float getRange();


	Light getLight();

	void translate(glm::vec3 trns);
	void setPosition(glm::vec3 pos);
	void setIntensity(glm::vec3 pow);
	void setAmbient(glm::vec3 ambient);
	void setRange(GLfloat rng);
	void setAngle(glm::vec3 angle);
	void setCone(GLfloat sz);

private:
	Light lightInfo;
};

