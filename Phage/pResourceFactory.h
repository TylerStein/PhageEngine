#pragma once
#include "pModel.h"
#include "pMaterial.h"
#include "pImage.h"
#include "pResourceManager.h"
#include "pModelManager.h"
#include "pPrimitiveMaker.h"
#include "pShaderManager.h"

class pResourceFactory
{
public:
	pResourceFactory();
	~pResourceFactory();

	//Functions to set the manager references
	void setModelManager(pModelManager* modelManager);
	void setMaterialManager(pMaterialManager* materialManager);
	void setImageManager(pImageManager* imageManager);
	void setShaderManager(pShaderManager* shaderManager);

	//Functions to create assets
	//Create a model
	pModel* createModel(std::string name, pMaterial* mat, GLfloat* vertPositions, GLfloat* vertNormals, GLfloat* vertColors, GLfloat* vertUVs, GLuint numVerts, GLenum drawMode);
	pModel* createPrimitive(std::string name, pPrimitiveMaker::Primitives type, glm::vec3 scale = glm::vec3(1), glm::vec3 color = glm::vec3(1));
	//Create a material
	pMaterial* createMaterial(std::string name, pShader* shader, MaterialInfo info);
	//Create an image
	pImage* createImage(std::string name, std::string filePath);
	pImage* createDebugImage(std::string name);
	//Create a shader
	pShader* createShader(std::string shaderName, attribNameMap attribs, uniformNameMap uniforms, std::string vertShaderPath, std::string fragShaderPath);

	//Functions to receive assets from cache or file
	//Retreive a model from file
	pModel* getModel(std::string name, std::string path);
	//Retreive an existing model
	pModel* getModel(std::string name);
	//Retreive a material from file
	pMaterial* getMaterial(std::string name, std::string path);
	//Retreive an existing material
	pMaterial* getMaterial(std::string name);
	//Retreive an image from file
	pImage* getImage(std::string name, std::string path);
	//Retreive an existing image
	pImage* getImage(std::string name);
	//Retreive existing shader
	pShader* getShader(std::string name);

private:
	//Manager references
	pModelManager* modelManager;
	pMaterialManager* materialManager;
	pImageManager* imageManager;
	pShaderManager* shaderManager;
};

