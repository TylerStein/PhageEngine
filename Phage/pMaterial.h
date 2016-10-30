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
	std::vector<pImage*> textureList;
	GLboolean bHasTexture;
};

class pMaterial : public pAsset
{
public:
	pType type;

	pMaterial(std::string name, pShader* matShader, MaterialInfo matInfo);
	~pMaterial();

	std::string getName() override;

	GLuint getShaderProgramID();
	ShaderInfo getShaderInfo();
	GLboolean hasTexture();
	void bindTextures();


private:
	std::string name;
	GLuint shaderProgramID;

	pShader* shader;
	MaterialInfo matInfo;
};

