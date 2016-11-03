#pragma once
#include "pAsset.h"

//ShaderInfo holds essential information on what the user's shader values are called
struct ShaderInfo {
public:
	ShaderInfo() {}
	ShaderInfo(GLchar* vertPosAttribute, GLchar* vertColorAttribute, GLchar* vertCoordAttribute, GLchar* vertNormalAttribute,
		GLchar* modelViewAttribute, GLchar* projectionAttribute, GLchar* viewAttribute, GLchar* pixelLightPositionAttribute, GLchar* pixelLightIntensityAttribute)
	{

		vertexPositionAttribute = vertPosAttribute;
		vertexColorAttribute = vertColorAttribute;
		vertexCoordinateAttribute = vertCoordAttribute;
		vertexNormalAttribute = vertNormalAttribute;
		this->modelViewAttribute = modelViewAttribute;
		this->projectionAttribute = projectionAttribute;
		this->viewAttribute = viewAttribute;
	}

	//The vertex position attribute name
	GLchar* vertexPositionAttribute;
	//The vertex color attribute name
	GLchar* vertexColorAttribute;
	//The texture UV coordinate attribute name
	GLchar* vertexCoordinateAttribute;
	//The texture normal coordinate attribute name
	GLchar* vertexNormalAttribute;
	//The model-view matrix uniform variable name
	GLchar* modelViewAttribute;
	//The camera projection matrix uniform variable name
	GLchar* projectionAttribute;
	//The camera view matrix uniform variable name
	GLchar* viewAttribute;
};

class pShader : public pAsset
{
public:
	pShader(std::string shaderName, std::string vertPath, std::string fragPath, ShaderInfo info);
	~pShader();

	void createShader(std::string vertPath, std::string fragPath);
	ShaderInfo getShaderInfo();

	GLuint getShaderProgramID() const;

	std::string getName();
private:
	std::string name;
	GLuint shaderProgramID;
	ShaderInfo shaderInfo;

};

