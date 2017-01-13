#include "pShader.h"
#include "pFileReader.h"
#include "GLError.h"

#ifdef _DEBUG
#include <iostream>
#endif



pShader::pShader(std::string name, attribNameMap attributeNames, uniformNameMap uniformNames, std::string vertexShaderPath, std::string fragmentShaderPath)
{
	if (fragmentShaderPath == "" || vertexShaderPath == "") {
		#ifdef _DEBUG
		std::cout << "Attempted to create shader without providing filepaths!" << std::endl;
		#endif
		LogManager::instance()->warning("Attempted to create shader without providing filepaths!");
		return;
	}

	//Set the name
	this->name = name;

	//Store the shader file paths
	vertPath = vertexShaderPath;
	fragPath = fragmentShaderPath;

	//Compile the shader
	compileShader();

	attributeMap = new attribLocMap();
	uniformMap = new uniformLocMap();

	for (int i = 0; i < attributeNames.Count(); ++i) {
		addAttribute(attributeNames.findKey(i), attributeNames.findValue(i));
	}

	for (int i = 0; i < uniformNames.Count(); ++i) {
		addUniform(uniformNames.findKey(i), uniformNames.findValue(i));
	}
}

pShader::~pShader()
{
	delete attributeMap;
	delete uniformMap;
}

void pShader::setUniformMat4(GLuint attributeID, glm::mat4 data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniformMatrix4fv(attributeID, 1, GL_FALSE, &data[0][0]);
}

void pShader::setUniformVec3(GLuint attributeID, glm::vec3 data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform3fv(attributeID, 1, &data[0]);
}

void pShader::setUniformVec2(GLuint attributeID, glm::vec2 data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform2fv(attributeID, 1, &data[0]);
}

void pShader::setUniformFloat(GLuint attributeID, GLfloat data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform1f(attributeID, data);
}

void pShader::setUniformInt(GLuint attributeID, GLint data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform1i(attributeID, data);
}

void pShader::setUniformTextureID(GLuint attributeID, GLuint textureID)
{	
	//Make sure the shader is being used
	glUseProgram(shaderID);

	glUniform1i(attributeID, textureID);
}

void pShader::setUniformVec3(GLuint attributeID, std::vector<glm::vec3> data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	//Create a vec3 array pointer from the incoming data
	const glm::vec3* arr = data.data();

	//Pass in the array
	glUniform3fv(attributeID, data.size(), &arr[0][0]);
}

void pShader::setUniformFloat(GLuint attributeID, std::vector<GLfloat> data)
{
	//Make sure the shader is being used
	glUseProgram(shaderID);

	//Create a float array pointer from the incoming data
	const GLfloat* arr = data.data();

	//Pass in the array
	glUniform1fv(attributeID, data.size(), &arr[0]);
}

bool pShader::hasAttribute(Attributes attrib)
{
	bool ans = (attributeFlags & attrib);
	return ans;
}

bool pShader::hasUniform(Uniforms uni)
{
	bool ans = (uniformFlags & uni);
	return ans;
}

GLuint pShader::getAttributeLocation(std::string shaderVar)
{
	return glGetAttribLocation(shaderID, (char*)shaderVar.c_str());
}

GLuint pShader::getUniformLocation(std::string shaderVar)
{
	return glGetUniformLocation(shaderID, (char*)shaderVar.c_str());
}

bool pShader::addAttribute(Attributes targetAttrib, std::string shaderVar)
{
	//Check if attribute already exists
	if (!hasAttribute(targetAttrib)) {
		//Get the attribute location
		GLuint varLocation = getAttributeLocation(shaderVar);

		//Check if location is valid
		if (varLocation != -1) {
			attributeMap->insert(targetAttrib, varLocation);
			attributeFlags = (attributeFlags | targetAttrib);
			return true;
		}
		else {
#ifdef _DEBUG
			std::cout << "Unable to find attribute (" << shaderVar << ") in shader" << std::endl;
			return false;
#endif
		}
	}

#ifdef _DEBUG
	std::cout << "Attribute already exists (" << shaderVar << ") in map" << std::endl;
#endif
	return false;
}

bool pShader::addUniform(Uniforms targetUniform, std::string shaderVar)
{
	//Check if uniform already exists
	if (!hasUniform(targetUniform)) {
		//Get the attribute location
		GLuint varLocation = getUniformLocation(shaderVar);

		//Check if location is valid
		if (varLocation != -1) {
			uniformMap->insert(targetUniform, varLocation);
			uniformFlags = (uniformFlags | targetUniform);
			return true;
		}
		else {
#ifdef _DEBUG
			std::cout << "Unable to find uniform (" << shaderVar << ") in shader" << std::endl;
			return false;
#endif
		}
	}
#ifdef _DEBUG
	std::cout << "Uniform already exists (" << shaderVar << ") in map" << std::endl;
#endif
	return false;
}

GLuint pShader::getAttributeID(Attributes attrib)
{
	GLint res = -1;

	if(hasAttribute(attrib)){
		res = attributeMap->findValue(attrib);
	}

	return res;
}

GLuint pShader::getUniformID(Uniforms uni)
{
	GLint res = -1;

	if (hasUniform(uni)) {
		res = uniformMap->findValue(uni);
	}

	return res;
}

void pShader::compileShader()
{
	pFileReader reader;

	//Read the shader files
	std::string vs = reader.readFile((char*)vertPath.c_str());
	std::string fs = reader.readFile((char*)fragPath.c_str());

	//Create char* of the shaders
	const char* vertex_shader = vs.c_str();
	const char* fragment_shader = fs.c_str();

	//Compile the vertex shader
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderID, 1, &vertex_shader, NULL);
	glCompileShader(vertShaderID);
	GLError::checkShaderError(vertShaderID, "VertexShader");

	//Compile the fragment shader
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderID, 1, &fragment_shader, NULL);
	glCompileShader(fragShaderID);
	GLError::checkShaderError(fragShaderID, "FragmentShader");

	//Create the shader program by attaching the vertex and fragment shaders
	shaderID = glCreateProgram();
	glAttachShader(shaderID, fragShaderID);
	glAttachShader(shaderID, vertShaderID);

	//Link the shaderID as the currently used shader
	glLinkProgram(shaderID);

	GLError::printActiveAttributes(shaderID);

	//Check for linking errors
	GLError::checkLinkError(shaderID);

	glDetachShader(shaderID, fragShaderID);
	glDetachShader(shaderID, vertShaderID);
}

GLuint pShader::getShaderID()
{
	return shaderID;
}

std::string pShader::getName()
{
	return name;
}

GLint pShader::getUniformFlags()
{
	return uniformFlags;
}

GLint pShader::getAttributeFlags()
{
	return attributeFlags;
}