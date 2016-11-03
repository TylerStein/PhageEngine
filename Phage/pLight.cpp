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
