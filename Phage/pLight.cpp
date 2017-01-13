#include "pLight.h"

pLight::pLight(Light::LightType type, glm::vec3 position, GLfloat range, glm::vec3 color, glm::vec3 intensity, glm::vec3 ambient, GLfloat attenuation)
{
	lightInfo.type = Light::LightType::POINT;
	lightInfo.position = position;
	lightInfo.color = color;
	lightInfo.intensity = intensity;
	lightInfo.attenuation = attenuation;
	lightInfo.ambient = ambient;
	lightInfo.range = range;
	lightInfo.cone = 0;
	lightInfo.eAngle = glm::vec3(0);
}

pLight::pLight(Light::LightType type, glm::vec3 angle, glm::vec3 color, glm::vec3 intensity, glm::vec3 ambient, GLfloat attenuation)
{
	lightInfo.type = Light::LightType::DIRECTIONAL;
	lightInfo.position = glm::vec3(0);
	lightInfo.color = color;
	lightInfo.intensity = intensity;
	lightInfo.attenuation = attenuation;
	lightInfo.ambient = ambient;
	lightInfo.range = -1;
	lightInfo.cone = -1;
	lightInfo.eAngle = glm::vec3(0);
}

pLight::pLight(Light::LightType type, glm::vec3 position, glm::vec3 angle, glm::vec3 color, GLfloat range, GLfloat cone, glm::vec3 intensity, glm::vec3 ambient, GLfloat attenuation)
{
	lightInfo.type = Light::LightType::SPOT;
	lightInfo.position = position;
	lightInfo.color = color;
	lightInfo.intensity = intensity;
	lightInfo.attenuation = attenuation;
	lightInfo.ambient = ambient;
	lightInfo.range = range;
	lightInfo.cone = cone;
	lightInfo.eAngle = glm::vec3(0);
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

glm::vec3 pLight::getAmbient()
{
	return lightInfo.ambient;
}

float pLight::getRange() {
	return lightInfo.range;
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

void pLight::setAmbient(glm::vec3 ambient)
{
	lightInfo.ambient = ambient;
}


void pLight::setRange(GLfloat rng) {
	lightInfo.range = rng;
}

void pLight::setAngle(glm::vec3 angle)
{
	lightInfo.eAngle = angle;
}
