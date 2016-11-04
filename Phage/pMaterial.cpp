#include "pMaterial.h"
#include "pFileReader.h"
#include "LogManager.h"
#include "GLError.h"
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

void pMaterial::setupTexture()
{
	GLubyte* textureData = matInfo.texture0->imageData;

	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, textureID[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, matInfo.texture0->getWidth(), matInfo.texture0->getHeight(), 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);
	GLError::printError(__FILE__, __LINE__);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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

GLuint pMaterial::getTexture0ID()
{
	return matInfo.texture0->getTextureID();
}

