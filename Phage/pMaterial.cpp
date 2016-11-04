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

void pMaterial::setupShader()
{
	//Make sure the shader is being used
	glUseProgram(getShaderProgramID());

	//Generate IDs for material attributes
	ambientID = glGetUniformLocation(getShaderProgramID(), shader->getShaderInfo().ambientAttribute);
	diffuseID = glGetUniformLocation(getShaderProgramID(), shader->getShaderInfo().diffuseAttribute);
	specularID = glGetUniformLocation(getShaderProgramID(), shader->getShaderInfo().specularAttribute);
	shininessID = glGetUniformLocation(getShaderProgramID(), shader->getShaderInfo().shininessAttribute);
	alphaID = glGetUniformLocation(getShaderProgramID(), shader->getShaderInfo().alphaAttribute);

	//Generate IDs for light position and power
	lightPosID = glGetUniformLocation(getShaderProgramID(), shader->getShaderInfo().lightPosAttribute);
	lightPowID = glGetUniformLocation(getShaderProgramID(), shader->getShaderInfo().lightPowAttribute);

	//Pass the ambient and diffuse attributes the appropriate information
	glUniform3f(ambientID, matInfo.ambient.r, matInfo.ambient.g, matInfo.ambient.b);
	glUniform3f(diffuseID, matInfo.diffuse.r, matInfo.diffuse.g, matInfo.diffuse.b);
	glUniform3f(specularID, matInfo.specular.r, matInfo.specular.g, matInfo.specular.b);
	glUniform1f(shininessID, matInfo.shininess);
	glUniform1f(alphaID, matInfo.alpha);
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

GLuint pMaterial::getDiffuseID()
{
	return diffuseID;
}

GLuint pMaterial::getAmbientID()
{
	return ambientID;
}

GLuint pMaterial::getSpecularID()
{
	return specularID;
}


GLuint pMaterial::getShininessID()
{
	return shininessID;
}

GLuint pMaterial::getAlphaID()
{
	return alphaID;
}

void pMaterial::setLightEffect(glm::vec3 pos, glm::vec3 pow)
{
	//Pass in the light position and power vec3 pair
	glUniform3f(lightPosID, pos.x, pos.y, pos.z);
	glUniform3f(lightPowID, pow.r, pow.g, pow.b);
}

