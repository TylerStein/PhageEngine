#pragma once
#include "pAsset.h"
#include "glm\vec3.hpp"
#include "pImage.h"
#include "pEnums.h"
#include <vector>

class pMaterial : public pAsset
{
public:
	pType type;

	pMaterial(std::string name, pImage *image);
	pMaterial(std::string name, std::vector<pImage*> images, GLuint blendMode);
	pMaterial(std::string name, glm::vec3 diffuseRGB);
	pMaterial(std::string name, glm::vec3 diffuseRGB, glm::vec3 ambientRGB, float shininess, float alpha);
	pMaterial(std::string name);
	pMaterial();
	~pMaterial();

	std::string getName() override;
	GLuint getID() override;

	GLuint getShaderProgramID();
	void loadShader(char* vertexShaderPath, char* fragmentShaderPath);
	GLboolean hasTexture();
	void bindTextures();



private:
	std::string name;
	GLuint shaderProgramID;
	GLuint ID;

	glm::vec3 diffuse;
	glm::vec3 ambient;
	float shininess;
	float alpha;
	GLuint blendMode;
	GLboolean bHasTexture;

	std::vector<pImage*> textures;
};

