#pragma once
#include "pAsset.h"
#include "glm\vec3.hpp"
#include "pImage.h"
#include "pShader.h"
#include "pEnums.h"
#include <vector>


struct MaterialInfo {
	glm::vec3 diffuse;
	glm::vec3 ambient;
	float shininess;
	float alpha;
	GLuint blendMode;
	pImage* texture0;
	GLboolean bHasTexture;
};

class pMaterial : public pAsset
{
public:
	pType type;

	pMaterial(std::string name, pShader* matShader, MaterialInfo matInfo);
	~pMaterial();

	std::string getName() override;

	void setupTexture();

	GLuint getShaderProgramID();
	ShaderInfo getShaderInfo();
	GLboolean hasTexture();
	GLuint getTexture0ID();

private:
	GLuint textureID[1];
	std::string name;
	pShader* shader;
	MaterialInfo matInfo;
};

