#pragma once
#include "pAsset.h"
#include "glm\vec3.hpp"

class pMaterial : public pAsset
{
public:
	pType type;

	std::string getName() override;
	GLuint getID() override;

	GLuint getShaderProgramID();
	void loadShader(char* vertexShaderPath, char* fragmentShaderPath);

	pMaterial();
	~pMaterial();

private:
	std::string name;
	GLuint shaderProgramID;
	GLuint ID;
	glm::vec3 ambient;
	float shininess;
	float alpha;

};

