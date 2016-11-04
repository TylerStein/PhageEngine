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
	glm::vec3 specular;
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
	void setupShader();

	GLuint getShaderProgramID();
	ShaderInfo getShaderInfo();
	GLboolean hasTexture();
	GLuint getTexture0ID();
	GLuint getDiffuseID();
	GLuint getAmbientID();
	GLuint getSpecularID();
	GLuint getShininessID();
	GLuint getAlphaID();

	void setLightEffect(glm::vec3 pos, glm::vec3 pow);

private:
	GLuint textureID[1];
	GLuint diffuseID;
	GLuint ambientID;
	GLuint specularID;
	GLuint shininessID;
	GLuint alphaID;
	GLuint lightPosID;
	GLuint lightPowID;
	std::string name;
	pShader* shader;
	MaterialInfo matInfo;
};

