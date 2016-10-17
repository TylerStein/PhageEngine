#include "pMaterial.h"
#include "pFileReader.h"
#include "LogManager.h"
#include <iostream>

std::string pMaterial::getName()
{
	return name;
}

GLuint pMaterial::getID()
{
	return ID;
}

GLuint pMaterial::getShaderProgramID()
{
	return shaderProgramID;
}

void pMaterial::loadShader(char * vertexShaderPath, char * fragmentShaderPath)
{
	pFileReader reader;

	//Read the shader files
	std::string vs = reader.readFile(vertexShaderPath);
	std::string fs = reader.readFile(fragmentShaderPath);

	//Create char* of the shaders
	const char* vertex_shader = vs.c_str();
	const char* fragment_shader = fs.c_str();

	//Load the shaders
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vertex_shader, NULL);
	glCompileShader(vertShaderID);

	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fragment_shader, NULL);
	glCompileShader(fragShaderID);

	

	//Create the shader program
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, fragShaderID);
	glAttachShader(shaderProgramID, vertShaderID);
	glLinkProgram(shaderProgramID);
	
	//Check for linking errors
	GLint isLinked = 0;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		printf("Shader linking error!\n");
		LogManager::instance()->error("Error linking shader!\n");
	}

}

GLboolean pMaterial::hasTexture()
{
	return bHasTexture;
}

void pMaterial::bindTextures()
{
	if (textures.size() > 0) {
		for (int i(0); i < textures.size(); ++i) {
			textures.at(i)->bindTexture();
		}
	}
}

pMaterial::pMaterial(std::string name)
	:pAsset(pType::MATERIAL)
{
	//Call the full constructor with fill-in values
	pMaterial(name, glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0), 1.0f, 1.0f);
}

pMaterial::pMaterial() : pAsset(pType::MATERIAL)
{
	pMaterial("INVALID");
}

pMaterial::pMaterial(std::string name, glm::vec3 diffuseRGB) : pAsset(pType::MATERIAL)
{
	//Call the full constructor with the given information and fill-ins
	pMaterial(name, diffuseRGB, glm::vec3(1.0, 1.0, 1.0), 1.0f, 1.0f);
}

pMaterial::pMaterial(std::string name, glm::vec3 diffuseRGB, glm::vec3 ambientRGB, float shininess, float alpha) : pAsset(pType::MATERIAL)
{
	this->name = name;
	//Set the diffuse
	diffuse = diffuseRGB;
	//Set the ambient
	ambient = ambientRGB;
	//Set the shininess value
	this->shininess = shininess;
	//Set the alpha/transparency value
	this->alpha = alpha;
	//Set the blend mode to additive
	this->blendMode = GL_ADD;
	//Declare the texture list as empty
	textures = std::vector<pImage*>(0);
	bHasTexture = false;
}

pMaterial::pMaterial(std::string name, pImage * image) : pAsset(pType::MATERIAL)
{	
	//Call the base constructor to fill in missing values
	pMaterial();
	//Set the name
	this->name = name;
	//Only one texture, declare the list as size 1
	textures = std::vector<pImage*>(1);
	//Add the single image to the texture list
	textures.push_back(image);
	bHasTexture = true;
}

pMaterial::pMaterial(std::string name, std::vector<pImage*> images, GLuint blendMode) : pAsset(pType::MATERIAL)
{
	//Call the base constructor to fill in missing values
	pMaterial();
	//Set the name
	this->name = name;
	//Define textures as the same size of images in
	textures = std::vector<pImage*>(images.size());
	//Copy over each element of the incoming list of images to our textures list
	for (int x(0); x < images.size(); ++x) {
		textures.push_back(images.at(x));
	}

	//Set the blend mode to the one supplied
	this->blendMode = blendMode;

	bHasTexture = true;
}


pMaterial::~pMaterial()
{

}
