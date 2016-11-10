#include "pMaterial.h"
#include "pFileReader.h"
#include "LogManager.h"
#include "GLError.h"
#include <iostream>


pMaterial::pMaterial(std::string name, pShader * matShader, MaterialInfo matInfo)
{
	this->name = name;

	//Use the incoming shader as this material's shader
	this->shader = matShader;

	this->matInfo = matInfo;

	//Send incoming data to the shader
	setDiffuseColor(matInfo.diffuse);
	setAmbientColor(matInfo.ambient);
	setSpecularColor(matInfo.specular);
	setShininess(matInfo.shininess);

	//Send incoming textures to the shader
	if(matInfo.diffuseTexture != NULL) { setDiffuseTexture(matInfo.diffuseTexture); }
	if(matInfo.specularTexture != NULL) { setSpecularTexture(matInfo.specularTexture); }
	if(matInfo.bumpTexture != NULL) { setBumpTexture(matInfo.bumpTexture); }
}

pMaterial::~pMaterial()
{

}


std::string pMaterial::getName()
{
	return name.c_str();
}

void pMaterial::setViewMatrices(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	//Make sure the shader is being used
	glUseProgram(getShaderProgramID());

	shader->setPropertyMat4(shader->getPropertyID(pShader::Model_View), modelMatrix);
	shader->setPropertyMat4(shader->getPropertyID(pShader::Camera_View), viewMatrix);
	shader->setPropertyMat4(shader->getPropertyID(pShader::Projection_View), projectionMatrix);
}

void pMaterial::setDiffuseColor(glm::vec3 diffuseColor)
{
	//Make sure the shader is being used
	glUseProgram(getShaderProgramID());

	shader->setPropertyVec3(shader->getPropertyID(pShader::DiffuseColor), diffuseColor);
}

void pMaterial::setSpecularColor(glm::vec3 specularColor)
{
	//Make sure the shader is being used
	glUseProgram(getShaderProgramID());

	shader->setPropertyVec3(shader->getPropertyID(pShader::SpecularColor), specularColor);
}

void pMaterial::setAmbientColor(glm::vec3 ambientColor)
{
	//Make sure the shader is being used
	glUseProgram(getShaderProgramID());

	shader->setPropertyVec3(shader->getPropertyID(pShader::AmbientColor), ambientColor);
}

void pMaterial::setShininess(GLfloat shininess)
{
	//Make sure the shader is being used
	glUseProgram(getShaderProgramID());

	shader->setPropertyFloat(shader->getPropertyID(pShader::Shininess), shininess);
}

void pMaterial::setDiffuseTexture(pImage * tex)
{
	tex->setupTexture(GL_TEXTURE0);
	shader->setPropertyTextureID(pShader::Attributes::DiffuseColor, tex->textureID);
}

void pMaterial::setSpecularTexture(pImage * tex)
{
	tex->setupTexture(GL_TEXTURE1);
	shader->setPropertyTextureID(pShader::Attributes::SpecularTexture, tex->textureID);
}

void pMaterial::setBumpTexture(pImage * tex)
{
	tex->setupTexture(GL_TEXTURE2);
	shader->setPropertyTextureID(pShader::Attributes::BumpTexture, tex->textureID);
}

GLuint pMaterial::getModelMatrixID()
{
	return shader->getPropertyID(pShader::Attributes::Model_View);
}

GLuint pMaterial::getViewMatrixID()
{
	return shader->getPropertyID(pShader::Attributes::Camera_View);
}

GLuint pMaterial::getProjectionMatrixID()
{
	return shader->getPropertyID(pShader::Attributes::Projection_View);
}


GLuint pMaterial::getShaderProgramID()
{
	return shader->getShaderID();
}


void pMaterial::setLightEffect(Light light)
{
	//if ((shader->getFlags() & pShader::Light_Affected) == pShader::Light_Affected) {
		//Make sure the shader is being used
		glUseProgram(getShaderProgramID());

		//Set all light properties
		shader->setPropertyVec3(shader->getPropertyID(pShader::Light_Position), light.position);
		shader->setPropertyVec3(shader->getPropertyID(pShader::Light_Color), light.color);
		shader->setPropertyVec3(shader->getPropertyID(pShader::Light_Power), light.intensity);
		shader->setPropertyFloat(shader->getPropertyID(pShader::Light_Ambient), light.ambientCoefficient);
		shader->setPropertyFloat(shader->getPropertyID(pShader::Light_Attenuation), light.attenuation);
	//}
}

void pMaterial::useTextures() {
	//Bind the texture to 0th slot for use this pass
	matInfo.diffuseTexture->useTexture(GL_TEXTURE0);
	
	//Potential bump and specular texture use
	//matInfo.bumpTexture->useTexture(GL_TEXTURE1);
	//matInfo.specularTexture->useTexture(GL_TEXTURE2);
}