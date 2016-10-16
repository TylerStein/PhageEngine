#include "pMaterial.h"
#include "pFileReader.h"

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
}

pMaterial::pMaterial()
	:pAsset(pType::MATERIAL)
{

}


pMaterial::~pMaterial()
{
}
