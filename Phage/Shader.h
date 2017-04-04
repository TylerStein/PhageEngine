#pragma once
#include "GL/glew.h"
#include "pEnums.h"
#include "pAsset.h"
#include <vector>
#include <map>

/*
enum ShaderAttributes {
	VertexPosition = 1 << 1,
	VertexNormal = 1 << 2,
	VertexCoordinate = 1 << 3,
	VertexColor = 1 << 4,
	VertexTangent = 1 << 5,
	VertexBiTangent = 1 << 6,
	VertexIndeces = 1 << 7,
	BoneData = 1 << 8,

	NONE = 1 << 16
};*/

//Base class for shader values
class ShaderValue {
	friend class Shader;

public:
	//Create a shader value extracted from a shader
	ShaderValue(GLchar* name, GLuint valueID, GLenum valueType, Attributes attrib = NONE);

	//Set data for this value
	void setData(GLuint size, GLvoid* data);

	GLvoid* data() const;
	GLuint id() const;
	GLenum valueType() const;
	GLchar* name() const;
	GLuint dataSize() const;
	Attributes attribute() const;
	bool isAttribute() const;
	bool hasAttribute(Attributes attrib);

protected:
	GLvoid* _data;
	GLuint _dataSize;
	GLchar* _name;
	GLuint _valueID;
	GLenum _valueType;
	Attributes _attribute;
};

class Shader : pAsset
{
	
public:
	//Supply array of: Directories[] and their shader types[], attribute names[] and their role[]
	Shader(std::vector<std::string> shaderDirectories, std::vector<GLenum> shaderTypes, std::vector<ShaderValue> attributeValues);
	~Shader();

	//Compile shader on to the GPU
	bool compileShader();

	//Update uniforms used by shader
	bool sendShaderUniforms();

	//Sets the value of a particular uniform
	bool sendShaderUniform(ShaderValue val);

	//Sets the value of a particular attribute (Creates and sends buffer if attributeBufferID is invalid)
	bool sendShaderAttribute(ShaderValue val, GLuint& attributeBufferID, GLuint dataOffset = 0U, GLenum usage = GL_STATIC_DRAW, GLenum targetBuffer = GL_ARRAY_BUFFER);

	//Print attributes and uniforms to console
	bool printShaderValues();

	//Delete the shader from the GPU
	bool deleteShader();

	//Update value data source (could hook up to any constant pointer)
	bool setValueData(GLchar* name, GLuint size, GLvoid* data);
	bool setValueData(GLuint valueID, GLuint size, GLvoid* data);
	bool setValueData(Attributes attrib, GLuint size, GLvoid* data);

	//Get shader value ID
	GLuint getValueID(GLchar* name) const;

	//Get shader value name
	GLchar* getValueName(GLuint valueID) const;

	//Get shader value existance
	bool getValueDoesExist(GLuint valueID) const;

	ShaderValue* getShaderValue(GLchar* name) const;
	ShaderValue* getShaderValue(GLuint valueID) const;
	ShaderValue* getShaderAttributeValue(Attributes attrib) const;

	//Get ID of value from shader
	GLuint getUniformFromShader(std::string valueName);
	GLuint getAttributeFromShader(std::string valueName);

	//Holds shader values
	std::vector<ShaderValue*> _shaderValues;

	//Holds file directory for shader types
	std::map<GLenum, std::string> _shaderDirectory;

	//Shader program ID
	GLuint _shaderID;

	void useShader();
	void unuseShader();

private:
	bool _isInUse;
};


