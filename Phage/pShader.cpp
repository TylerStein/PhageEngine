#include "pShader.h"
#include "pFileReader.h"
#include "GLError.h"
#include <iostream>

pShader::pShader(std::string shaderName, std::string vertPath, std::string fragPath, ShaderInfo info)
{
	shaderInfo = info;
	name = shaderName;
	if (vertPath != "" && fragPath != "") {
		createShader(vertPath, fragPath);
	}
	else {
		std::cout << "Invalid shader paths!" << std::endl;
	}
}

pShader::~pShader()
{
}

void pShader::createShader(std::string vertPath, std::string fragPath)
{
	pFileReader reader;

	//Read the shader files
	std::string vs = reader.readFile((char*)vertPath.c_str());
	std::string fs = reader.readFile((char*)fragPath.c_str());

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
	GLError::checkLinkError(shaderProgramID);
}

ShaderInfo pShader::getShaderInfo()
{
	return shaderInfo;
}

GLuint pShader::getShaderProgramID() const
{
	return shaderProgramID;
}

std::string pShader::getName()
{
	return name;
}
