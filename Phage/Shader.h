#pragma once
#include "GL/glew.h"
#include "pAsset.h"
#include <vector>
#include <map>


//Base class for shader values
class ShaderValue {
	friend class Shader;
public:
	//Create a shader value extracted from a shader
	ShaderValue(GLchar* name, GLuint valueID, GLenum valueType, bool isAttribute = false);

	//Set data for this value
	void setData(GLuint size, GLvoid* data);

	GLvoid* data() const;
	GLuint id() const;
	GLenum valueType() const;
	GLchar* name() const;
	bool isAttribute() const;



protected:
	GLvoid* _data;
	GLuint _dataSize;
	GLchar* _name;
	GLuint _valueID;
	GLenum _valueType;
	bool _isAttribute;
};

class Shader : pAsset
{
public:
	//Constructor takes array of directories and an array of associated shadertypes
	Shader(std::vector<std::string> directories = std::vector<std::string>(), std::vector<GLenum> types = std::vector<GLenum>());
	~Shader();

	//Compile shader on to the GPU
	bool compileShader();

	//Update uniforms used by shader
	bool updateShaderUniforms();
	bool updateShaderUniform(ShaderValue val);

	//Print attributes and uniforms to console
	bool printShaderValues();

	//Delete the shader from the GPU
	bool deleteShader();

	//Update value data source (could hook up to any constant pointer)
	bool setValueData(GLchar* name, GLuint size, GLvoid* data);
	bool setValueData(GLuint valueID, GLuint size, GLvoid* data);

	//Get shader value ID
	GLuint getValueID(GLchar* name) const;

	//Get shader value name
	GLchar* getValueName(GLuint valueID) const;

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


