#include "Shader.h"
#include "pFileReader.h"
#include "GLError.h"
#include <string>


Shader::Shader(std::vector<std::string> shaderDirectories, std::vector<GLenum> shaderTypes, std::vector<ShaderValue> attributeValues)
{
	//Holds shader values
	_shaderValues = std::vector<ShaderValue*>();
	for(int i = 0; i < attributeValues.size(); i++){
		_shaderValues.push_back(new ShaderValue(attributeValues[i]));
	}

	//Holds file directory for shader types
	_shaderDirectory = std::map<GLenum, std::string>();
	for (int i = 0; i < shaderDirectories.size(); i++) {
		_shaderDirectory.emplace(shaderTypes[i], shaderDirectories[i]);
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

	_shaderValues.clear();
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

	return true;
}

bool Shader::sendShaderUniforms()
{
	if (!_isInUse) {
		useShader();
	}

	for (int i = 0; i < _shaderValues.size(); i++) {
		sendShaderUniform(*_shaderValues[i]);
	}

	unuseShader();
	return true;
}

#define PRINT_VALUES true
bool Shader::sendShaderUniform(ShaderValue val)
{
	if (val.isAttribute() == false && _isInUse) {
		switch (val.valueType()) {
		case GL_INT:
			if (PRINT_VALUES) { printf("Set Uniform[%i] (Int) = %i", val.id(), (int)val.data()); }
			glUniform1i(val.id(), (GLint)val.data());
			return true;
		case GL_FLOAT:
			if (PRINT_VALUES) { printf("Set Uniform[%i] (Float) = %f", val.id(), (float*)val.data()); }
			glUniform1f(val.id(),  *(GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC2:
			if (PRINT_VALUES) { printf("Set Uniform[%i] (Vec2f) = %f", val.id(), (float*)val.data()); }
			glUniform2fv(val.id(), 1, (GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC3:
			if (PRINT_VALUES) { printf("Set Uniform[%i] (Vec3f) = %f", val.id(), (float*)val.data()); }
			glUniform3fv(val.id(), 1, (GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC4:
			if (PRINT_VALUES) { printf("Set Uniform[%i] (Vec4f) = %f", val.id(), (float*)val.data()); }
			glUniform4fv(val.id(), 1, (GLfloat*)val.data());
			return true;
		case GL_MATRIX3_NV:
			if (PRINT_VALUES) { printf("Set Uniform[%i] (Mat3NV) = %f", val.id(), (float*)val.data()); }
			glUniformMatrix3fv(val.id(), 1, GL_FALSE, (GLfloat*)val.data());
			return true;
		case GL_MATRIX4_NV:
			if (PRINT_VALUES) { printf("Set Uniform[%i] (Mat4NV) = %f", val.id(), (float*)val.data()); }
			glUniformMatrix4fv(val.id(), 1, GL_FALSE, (GLfloat*)val.data());
			return true;
		}
	}
	else {
#ifdef _DEBUG
		printf("Attempted to set uniform: Value is attribute or ShaderProgram is not in use");
#endif
	}
	
	return	false;
}

bool Shader::sendShaderAttribute(ShaderValue val, GLuint& attributeBufferID, GLuint dataOffset, GLenum usage, GLenum targetBuffer)
{
	if (val.isAttribute() == true && _isInUse && getValueDoesExist(val.id())) {
		switch (val.valueType()) {
		case GL_INT:
			if (PRINT_VALUES) { printf("Set Attribute[%i] (Int) = %i", val.id(), (int)val.data()); }

			//Create and bind buffer array  using attributeBufferID if it doesn't already exist
			if (attributeBufferID == 0) {
				glGenBuffers(1, &attributeBufferID);
				glBufferData(targetBuffer, val.dataSize(), val.data(), usage);
			}

			//Fill buffer data array
			if (attributeBufferID != 0) {
				glEnableVertexAttribArray(attributeBufferID);
				glVertexAttribPointer(attributeBufferID, 1, val.valueType(), GL_FALSE, sizeof(GLint), (GLvoid*)dataOffset);

			}

			return true;
		case GL_FLOAT:
			if (PRINT_VALUES) { printf("Set Attribute[%i] (Float) = %f", val.id(), (float*)val.data()); }
			glUniform1f(val.id(), *(GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC2:
			if (PRINT_VALUES) { printf("Set Attribute[%i] (Vec2f) = %f", val.id(), (float*)val.data()); }
			glUniform2fv(val.id(), 1, (GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC3:
			if (PRINT_VALUES) { printf("Set Attribute[%i] (Vec3f) = %f", val.id(), (float*)val.data()); }
			glUniform3fv(val.id(), 1, (GLfloat*)(val.data()));
			return true;
		case GL_FLOAT_VEC4:
			if (PRINT_VALUES) { printf("Set Attribute[%i] (Vec4f) = %f", val.id(), (float*)val.data()); }
			glUniform4fv(val.id(), 1, (GLfloat*)val.data());
			return true;
		case GL_MATRIX3_NV:
			if (PRINT_VALUES) { printf("Set Attribute[%i] (Mat3NV) = %f", val.id(), (float*)val.data()); }
			glUniformMatrix3fv(val.id(), 1, GL_FALSE, (GLfloat*)val.data());
			return true;
		case GL_MATRIX4_NV:
			if (PRINT_VALUES) { printf("Set Attribute[%i] (Mat4NV) = %f", val.id(), (float*)val.data()); }
			glUniformMatrix4fv(val.id(), 1, GL_FALSE, (GLfloat*)val.data());
			return true;
		}
	}
	else {
#ifdef _DEBUG
		printf("Attempted to set Attribute: Value is uniform or ShaderProgram is not in use");
#endif
	}

	return	false;
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

bool Shader::setValueData(ShaderAttributes attrib, GLuint size, GLvoid * data)
{
	for (int i = 0; i < _shaderValues.size(); i++) {
		if (_shaderValues[i]->hasAttribute(attrib)) {
			_shaderValues[i]->setData(size, data);
			return true;
		}
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

bool Shader::getValueDoesExist(GLuint valueID) const
{
	return (valueID < _shaderValues.size());
}

ShaderValue * Shader::getShaderValue(GLchar * name) const
{
	return getShaderValue(getValueID(name));
}

ShaderValue * Shader::getShaderValue(GLuint valueID) const
{
	return _shaderValues[valueID];
}

ShaderValue * Shader::getShaderAttributeValue(ShaderAttributes attrib) const
{
	for (GLuint i = 0; i < _shaderValues.size(); i++) {
		if (_shaderValues[i]->hasAttribute(attrib)) {
			return _shaderValues[i];
		}
	}

	return nullptr;
}

GLuint Shader::getUniformFromShader(std::string valueName)
{
	return glGetUniformLocation(_shaderID, (char*)valueName.c_str());
}

GLuint Shader::getAttributeFromShader(std::string valueName)
{
	return glGetAttribLocation(_shaderID, (char*)valueName.c_str());
}

ShaderValue::ShaderValue(GLchar* name, GLuint valueID, GLenum valueType, ShaderAttributes attrib)
{
	_name = name;
	_valueID = valueID;
	_valueType = valueType;
	_data = nullptr;
	_dataSize = 0;
	_attribute = attrib;
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
	bool res = (_attribute & NONE);
	return !res;
}

bool ShaderValue::hasAttribute(ShaderAttributes attrib)
{
	return false;
}

GLuint ShaderValue::dataSize() const
{
	return _dataSize;
}

ShaderAttributes ShaderValue::attribute() const
{
	return _attribute;
}

ShaderValue::ShaderValue(GLchar * name, GLuint valueID, GLenum valueType, ShaderAttributes attribs)
{
}

void ShaderValue::setData(GLuint size, GLvoid * data)
{
	_data = data;
	_dataSize = size;
}