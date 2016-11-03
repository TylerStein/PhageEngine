#pragma once
#include "glm/vec3.hpp"

class pLight
{
public:
	pLight();
	pLight(glm::vec3 position, glm::vec3 intensity);
	~pLight();

private:
	glm::vec3 intensity;
	glm::vec3 position;
};

