#include "Shader.h"
#include "pFileReader.h"
#include "GLError.h"
#include <string>

Shader::Shader(std::vector<std::string> directories, std::vector<GLenum> types)
{
	//Holds shader values
	_shaderValues = std::vector<ShaderValue*>();

	//Holds file directory for shader types
	_shaderDirectory = std::map<GLenum, std::string>();
	for (int i = 0; i < directories.size(); i++) {
		_shaderDirectory.emplace(types[i], directories[i]);
	}

	_isInUse = false;
}

Shader::~Shader()
{
	_shaderValues.clear();
	_shaderDirectory.clear();
}

bool Shader::compileShader()
{
	pFileReader reader;

	GLuint programID = 0;

	std::vector<GLuint> shaderIDs = std::vector<GLuint>();

	//Iterate through provided shader paths
	for (auto iter = _shaderDirectory.begin(); iter != _shaderDirectory.end(); iter++) {
		//Read in shader code
		const char* shaderData = reader.readFile((char*)iter->second.c_str()).c_str();

		//Create shader based on shader type
		GLuint shaderID = glCreateShader(iter->first);
		shaderIDs.push_back(shaderID);

		glShaderSource(shaderID, 1, &shaderData, NULL);
		glCompileShader(shaderID);
		std::string errorString = std::string("Shader Enum ") + std::to_string((GLuint)iter->first);
		GLError::checkShaderError(shaderID, errorString);
	}

	programID = glCreateProgram();
	for (int i = 0; i < shaderIDs.size(); i++) {
		glAttachShader(programID, shaderIDs[i]);
	}
	glLinkProgram(programID);
	
	//Print active values
	printShaderValues();

	//Check for linking errors
	GLError::checkLinkError(programID);

	for (int i = 0; i < shaderIDs.size(); i++) {
		glDetachShader(programID, shaderIDs[i]);
	}

	/*
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
	glDetachShader(shaderID, vertShaderID);*/

	return true;
}

bool Shader::updateShaderUniforms()
{
	if (!_isInUse) {
		useShader();
	}

	for (int i = 0; i < _shaderValues.size(); i++) {
		updateShaderUniform(*_shaderValues[i]);
	}

	unuseShader();
	return true;
}

bool Shader::updateShaderUniform(ShaderValue val)
{
	if (val.isAttribute() == false && _isInUse) {
		switch (val.valueType()) {
		case GL_INT:
			glUniform1i(val.id(), (GLint)val.data());
			return true;
		case GL_FLOAT:
			glUniform1f(val.id(),  *(GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC2:
			glUniform2fv(val.id(), 1, (GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC3:
			glUniform3fv(val.id(), 1, (GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC4:
			glUniform4fv(val.id(), 1, (GLfloat*)val.data());
			return true;
		case GL_MATRIX3_NV:
			glUniformMatrix3fv(val.id(), 1, GL_FALSE, (GLfloat*)val.data());
			return true;
		case GL_MATRIX4_NV:
			glUniformMatrix4fv(val.id(), 1, GL_FALSE, (GLfloat*)val.data());
			return true;
		}
	}
	return false;
}

bool Shader::printShaderValues()
{
	GLError::printActiveAttributes(_shaderID, name);
	GLError::printActiveUniforms(_shaderID, name);
	return true;
}

bool Shader::deleteShader()
{
	if (_isInUse) { unuseShader(); }
	glDeleteProgram(_shaderID);
	return true;
}

bool Shader::setValueData(GLchar * name, GLuint size, GLvoid * data)
{
	return setValueData(getValueID(name), size, data);
	return false;
}

bool Shader::setValueData(GLuint valueID, GLuint size, GLvoid * data)
{
	if (valueID < _shaderValues.size()) {
		_shaderValues[valueID]->setData(size, data);
		return true;
	}

	return false;
}

void Shader::useShader()
{
	_isInUse = true;
}

void Shader::unuseShader()
{
	_isInUse = false;
}

GLuint Shader::getValueID(GLchar * name) const
{
	for (GLuint i = 0; i < _shaderValues.size(); i++) {
		if (_shaderValues[i]->name() == name) {
			return i;
		}
	}

	return 0U;
}

GLchar * Shader::getValueName(GLuint valueID) const
{
	return _shaderValues[valueID]->name();
}

ShaderValue::ShaderValue(GLchar * name, GLuint valueID, GLenum valueType, bool isAttribute)
{
	_name = name;
	_valueID = valueID;
	_valueType = valueType;
	_isAttribute = isAttribute;
	_data = nullptr;
}

GLvoid * ShaderValue::data() const
{
	return _data;
}

GLuint ShaderValue::id() const
{
	return _valueID;
}

GLenum ShaderValue::valueType() const
{
	return _valueType;
}

GLchar * ShaderValue::name() const
{
	return _name;
}

bool ShaderValue::isAttribute() const
{
	return _isAttribute;
}

void ShaderValue::setData(GLuint size, GLvoid * data)
{
	_data = data;
	_dataSize = size;
}