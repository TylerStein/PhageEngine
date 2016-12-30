#pragma once
#include "glm/vec3.hpp"
#include "GL/glew.h"


//Light struct, gets passed to the shader
struct Light {
	float attenuation;
	float range;
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 ambient;
	glm::vec3 intensity;
};

class pLight
{
public:
	pLight();
	pLight(glm::vec3 position, GLfloat range = 25.0f, glm::vec3 color = glm::vec3(1.0f), glm::vec3 intensity = glm::vec3(1.0f), glm::vec3 ambient = glm::vec3(0.1f), GLfloat attenuation = 1.0f);
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

private:
	Light lightInfo;
};

