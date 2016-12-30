#include "pMaterial.h"
#include "pFileReader.h"
#include "LogManager.h"
#include "GLError.h"
#include "pLightManager.h"
#include <iostream>


pMaterial::pMaterial(std::string name, pShader * matShader, MaterialInfo matInfo)
{
	this->name = name;

	//Use the incoming shader as this material's shader
	this->shader = matShader;
	
	//Copy over the material info
	mat.copyFrom(matInfo);

	//Set up incoming textures
	if (mat.diffuseTexture != NULL) { setDiffuseTexture(mat.diffuseTexture, false); }
	if (mat.specularTexture != NULL) { setSpecularTexture(mat.specularTexture, false); }
	if (mat.bumpTexture != NULL) { setBumpTexture(mat.bumpTexture, false); }
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
	shader->setUniformMat4(shader->getUniformID(Uniforms::Model_View), modelMatrix);
	shader->setUniformMat4(shader->getUniformID(Uniforms::Camera_View), viewMatrix);
	shader->setUniformMat4(shader->getUniformID(Uniforms::Projection_View), projectionMatrix);
}

void pMaterial::setDiffuseColor(glm::vec3 diffuseColor, bool sendToShader)
{
	if (sendToShader) {
		if (shader->hasUniform(Uniforms::DiffuseColor)) {
			shader->setUniformVec3(shader->getUniformID(Uniforms::DiffuseColor), diffuseColor);
		}
	}
	else {
		mat.diffuse = diffuseColor;
	}
}

void pMaterial::setSpecularColor(glm::vec3 specularColor, bool sendToShader)
{
	if (sendToShader) {
	if (shader->hasUniform(Uniforms::SpecularColor)) {
		shader->setUniformVec3(shader->getUniformID(Uniforms::SpecularColor), specularColor);
	}
	}
	else {
		mat.specular = specularColor;
	}
}

void pMaterial::setAmbientColor(glm::vec3 ambientColor, bool sendToShader)
{
	if (sendToShader) {
	if (shader->hasUniform(Uniforms::AmbientColor)) {
		shader->setUniformVec3(shader->getUniformID(Uniforms::AmbientColor), ambientColor);
	}
	}
	else {
		mat.ambient = ambientColor;
	}
}

void pMaterial::setShininess(GLfloat shininess, bool sendToShader)
{
	if (sendToShader) {
	if (shader->hasUniform(Uniforms::Shininess)) {
		shader->setUniformFloat(shader->getUniformID(Uniforms::Shininess), shininess);
	}
	}
	else {
		mat.shininess = shininess;
	}
}

void pMaterial::setDiffuseTexture(pImage * tex, bool sendToShader)
{
	if (sendToShader) {
	if (shader->hasUniform(Uniforms::DiffuseTexture)) {
		shader->setUniformTextureID(shader->getUniformID(Uniforms::DiffuseTexture), tex->textureID);
	}
	}
	else {
		mat.diffuseTexture = tex;
		tex->setupTexture(GL_TEXTURE0);
	}
}

void pMaterial::setSpecularTexture(pImage * tex, bool sendToShader)
{
	if (sendToShader) {
	if (shader->hasUniform(Uniforms::SpecularTexture)) {

	shader->setUniformTextureID(shader->getUniformID(Uniforms::SpecularTexture), tex->textureID);
	}
	}
	else {
		mat.specularTexture = tex;
		tex->setupTexture(GL_TEXTURE1);
	}
}

void pMaterial::setEmission(glm::vec3 emissionColor, bool sendToShader) 
{
	if (sendToShader) {
	if (shader->hasUniform(Uniforms::Emission)) {
		shader->setUniformVec3(shader->getUniformID(Uniforms::Emission), emissionColor);
	}
	}
	else {
		mat.emission = emissionColor;
	}
}

void pMaterial::setBumpTexture(pImage * tex, bool sendToShader)
{
	if (sendToShader) {
	if (shader->hasUniform(Uniforms::BumpTexture)) {
		tex->setupTexture(GL_TEXTURE2);
		shader->setUniformTextureID(shader->getUniformID(Uniforms::BumpTexture), tex->textureID);
	}
	}
	else {
		mat.bumpTexture = tex;
	}
}

void pMaterial::setLightSources(std::vector<pLight*> lights)
{
	if (mat.useLight) {
		//Make sure the shader is being used
		glUseProgram(getShaderProgramID());

		std::vector<glm::vec3> aPos = std::vector<glm::vec3>();
		std::vector<glm::vec3> aColor = std::vector<glm::vec3>();
		std::vector<glm::vec3> aAmb = std::vector<glm::vec3>();
		std::vector<glm::vec3> aPow = std::vector<glm::vec3>();
		std::vector<GLfloat> aRange = std::vector<GLfloat>();
		std::vector<GLfloat> aAtten = std::vector<GLfloat>();

		for (int i = 0; i < lights.size(); ++i) {
			pLight* cl = lights.at(i);

			aPos.push_back(cl->getPosition());
			aColor.push_back(cl->getColor());
			aAmb.push_back(cl->getAmbient());
			aPow.push_back(cl->getIntensity());
			aRange.push_back(cl->getRange());
			aAtten.push_back(cl->getAttenuation());
		}

		//Set all light properties
		shader->setUniformInt(shader->getUniformID(Uniforms::Light_Count), lights.size());
		shader->setUniformVec3(shader->getUniformID(Uniforms::Light_Position), aPos);
		shader->setUniformVec3(shader->getUniformID(Uniforms::Light_Color), aColor);
		shader->setUniformVec3(shader->getUniformID(Uniforms::Light_Power), aAmb);
		shader->setUniformVec3(shader->getUniformID(Uniforms::Light_Ambient), aPow);
		shader->setUniformFloat(shader->getUniformID(Uniforms::Light_Range), aRange);
		shader->setUniformFloat(shader->getUniformID(Uniforms::Light_Attenuation), aAtten);
	}
}

GLuint pMaterial::getModelMatrixID()
{
	return shader->getUniformID(Uniforms::Model_View);
}

GLuint pMaterial::getViewMatrixID()
{
	return shader->getUniformID(Uniforms::Camera_View);
}

GLuint pMaterial::getProjectionMatrixID()
{
	return shader->getUniformID(Uniforms::Projection_View);
}


GLuint pMaterial::getShaderProgramID()
{
	return shader->getShaderID();
}

void pMaterial::useMaterial()
{
	

	//Send incoming data to the shader
	if (&mat.diffuse != NULL || shader->hasUniform(Uniforms::DiffuseColor)) { setDiffuseColor(mat.diffuse, true); }
	if (&mat.ambient != NULL || shader->hasUniform(Uniforms::AmbientColor)) { setAmbientColor(mat.ambient, true); }
	if (&mat.specular != NULL || shader->hasUniform(Uniforms::SpecularColor)) { setSpecularColor(mat.specular, true); }
	if (&mat.shininess != NULL || shader->hasUniform(Uniforms::Shininess)) { setShininess(mat.shininess, true); }
	if (&mat.emission != NULL || shader->hasUniform(Uniforms::Emission)) { setEmission(mat.emission, true); }

	//Send incoming textures to the shader
	if (mat.diffuseTexture != NULL) { mat.diffuseTexture->useTexture(GL_TEXTURE0); }
	if (mat.specularTexture != NULL) { mat.specularTexture->useTexture(GL_TEXTURE1); }
	if (mat.bumpTexture != NULL) { mat.bumpTexture->useTexture(GL_TEXTURE2); }

	if(mat.useLight == true){
		std::vector<pLight*> lights = pLightManager::instance()->getAllLights();
		setLightSources(lights);
	}
}

void pMaterial::unuseMaterial()
{
	//Unuse the textures
	if (mat.diffuseTexture != NULL) { mat.diffuseTexture->unuseTexture(GL_TEXTURE0); }
	if (mat.specularTexture != NULL) { mat.specularTexture->unuseTexture(GL_TEXTURE1); }
	if (mat.bumpTexture != NULL) { mat.bumpTexture->unuseTexture(GL_TEXTURE2); }
}

pShader * pMaterial::getShader()
{
	return shader;
}
