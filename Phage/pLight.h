#pragma once
#include "glm/vec3.hpp"

class pLight
{
public:
	pLight();
	pLight(glm::vec3 position, glm::vec3 intensity);
	~pLight();

	glm::vec3 getPosition();
	glm::vec3 getPower();

	void translate(glm::vec3 trns);
	void setPosition(glm::vec3 pos);
	void setPower(glm::vec3 pow);

private:
	glm::vec3 intensity;
	glm::vec3 position;
};

