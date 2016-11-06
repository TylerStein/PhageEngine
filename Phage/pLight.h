#pragma once
#include "glm/vec3.hpp"
#include "GL/glew.h"


//Light struct, gets passed to the shader
struct Light {
	float attenuation;
	float ambientCoefficient;
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 intensity;
};

class pLight
{
public:
	pLight();
	pLight(glm::vec3 position, glm::vec3 color, glm::vec3 intensity, GLfloat attenuation, GLfloat ambientCoefficient);
	~pLight();

	glm::vec3 getPosition();
	glm::vec3 getIntensity();
	glm::vec3 getColor();
	float getAttenuation();
	float getAmbientCoefficient();


	Light getLight();

	void translate(glm::vec3 trns);
	void setPosition(glm::vec3 pos);
	void setIntensity(glm::vec3 pow);
	void setAttenuation(GLfloat atten);
	void setAmbientCoefficient(GLfloat ambCoef);

private:
	Light lightInfo;
};

