#include "pLight.h"


pLight::pLight()
{
	lightInfo.position = glm::vec3(1);
	lightInfo.color = glm::vec3(1);
	lightInfo.intensity = glm::vec3(1);
	lightInfo.attenuation = 1.0f;
	lightInfo.ambientCoefficient = 1.0f;
}


pLight::pLight(glm::vec3 position, glm::vec3 color, glm::vec3 intensity, GLfloat attenuation, GLfloat ambientCoefficient)
{
	lightInfo.position = position;
	lightInfo.intensity = intensity;
	lightInfo.attenuation = attenuation;
	lightInfo.ambientCoefficient = ambientCoefficient;
}

pLight::~pLight()
{
}

glm::vec3 pLight::getPosition()
{
	return lightInfo.position;
}

glm::vec3 pLight::getIntensity()
{
	return lightInfo.intensity;
}

glm::vec3 pLight::getColor()
{
	return lightInfo.color;
}

float pLight::getAttenuation()
{
	return lightInfo.attenuation;
}

float pLight::getAmbientCoefficient()
{
	return lightInfo.ambientCoefficient;
}

Light pLight::getLight()
{
	return lightInfo;
}

void pLight::translate(glm::vec3 trns)
{
	lightInfo.position += trns;
}

void pLight::setPosition(glm::vec3 pos)
{
	lightInfo.position = pos;
}

void pLight::setIntensity(glm::vec3 intensity)
{
	lightInfo.intensity = intensity;
}

void pLight::setAttenuation(GLfloat atten)
{
	lightInfo.attenuation = atten;
}

void pLight::setAmbientCoefficient(GLfloat ambCoef)
{
	lightInfo.ambientCoefficient = ambCoef;
}
