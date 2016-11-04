#include "pLight.h"



pLight::pLight()
{
	intensity = glm::vec3(1);
	position = glm::vec3(0);
}


pLight::pLight(glm::vec3 position, glm::vec3 intensity)
{
	this->position = position;
	this->intensity = intensity;
}

pLight::~pLight()
{
}

glm::vec3 pLight::getPosition()
{
	return position;
}

glm::vec3 pLight::getPower()
{
	return intensity;
}

void pLight::translate(glm::vec3 trns)
{
	position += trns;
}

void pLight::setPosition(glm::vec3 pos)
{
	position = pos;
}

void pLight::setPower(glm::vec3 pow)
{
	intensity = pow;
}
