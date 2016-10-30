#include "pMaterial.h"
#include "pFileReader.h"
#include "LogManager.h"
#include <iostream>


pMaterial::pMaterial(std::string name, pShader * matShader, MaterialInfo matInfo)
{
	this->name = name;
	this->shader = matShader;
	this->matInfo = matInfo;
}

pMaterial::~pMaterial()
{

}


std::string pMaterial::getName()
{
	return name.c_str();
}


GLuint pMaterial::getShaderProgramID()
{
	return shader->getShaderProgramID();
}

ShaderInfo pMaterial::getShaderInfo()
{
	return shader->getShaderInfo();
}

GLboolean pMaterial::hasTexture()
{
	return matInfo.bHasTexture;
}

void pMaterial::bindTextures()
{
	if (matInfo.bHasTexture) {
		for (int i(0); i < matInfo.textureList.size(); ++i) {
			matInfo.textureList.at(i)->bindTexture();
		}
	}
}

