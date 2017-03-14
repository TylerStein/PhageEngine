#include "pResourceManager.h"
#include "pShader.h"

#pragma once
class pShaderManager
{
public:
	typedef pResourceHandle<pShader> HandleType;

	pShaderManager();
	~pShaderManager();

	//Creates a shader from provided info
	HandleType createShader(std::string shaderName, attribNameMap attribs, uniformNameMap uniforms, std::string vertShaderPath, std::string fragShaderPath);

	//Adds a shader to the manager
	HandleType addShader(std::string shaderName, pShader* shader);

	//Retreives a shader from the manager
	pShader* getShader(HandleType &handle);
	pShader* getShader(std::string shaderName);

	std::vector<pShader*> getAllShaders();

	//Delete a shader from the manager
	void deleteShader(std::string shaderName);

	//Clear all shaders from the manager
	void clear();

private:
	pResourceManager<pShader> shaderResources;

};

