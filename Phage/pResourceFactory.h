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
	static pResourceFactory* instance();

	//Functions to set the manager references
	void setModelManager(pModelManager* modelManager);
	void setMaterialManager(pMaterialManager* materialManager);
	void setImageManager(pImageManager* imageManager);
	void setShaderManager(pShaderManager* shaderManager);

	//Functions to create assets
	//Create a model
	pModel* createModel(std::string name, pMaterial* mat, GLfloat* vertPositions, GLfloat* vertNormals, GLfloat* vertColors, GLfloat* vertUVs, GLuint numVerts, GLenum drawMode);
	pModel* createModel(std::string name, pMaterial * material, GLenum drawMode, std::vector<GLfloat> vPositions,
		std::vector<GLuint> vIndeces = std::vector<GLuint>(),
		std::vector<GLfloat> vCoordinates = std::vector<GLfloat>(),
		std::vector<GLfloat> vNormals = std::vector<GLfloat>(),
		std::vector<GLfloat> vTangents = std::vector<GLfloat>(),
		std::vector<GLfloat> vBiTangents = std::vector<GLfloat>(),
		std::vector<GLfloat> vColors = std::vector<GLfloat>());
	
	//Create a material
	pMaterial* createMaterial(std::string name, pShader* shader, MaterialInfo info);
	//Create an image
	pImage* createImage(std::string name, std::string filePath);
	//Create a shader
	pShader* createShader(std::string shaderName, attribNameMap attribs, uniformNameMap uniforms, std::string vertShaderPath, std::string fragShaderPath);

	//Functions to receive assets from cache or file
	//Retreive a model from file
	pModel* loadModel(std::string name, std::string path, pMaterial* mat = nullptr);
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

	
	//Primitive/Debug accessors
	pModel* createPrimitiveShape(std::string name, pPrimitiveMaker::Primitives prim, glm::vec3 scale = glm::vec3(1), glm::vec3 color = glm::vec3(1), pMaterial* customMaterial = nullptr);

	//2x2 pixel checkerboard
	pImage* createDebugImage(std::string name);

	//White unlit material
	pMaterial* createDebugMaterial();

	/*==Debug Shader==
	@Attributes vPosition, vNormal, vCoordinate, vColor
	@Uniforms	cameraView, projectionView, modelView, diffuseColor
	*/
	pShader* createDebugShader();

private:
	pResourceFactory();
	~pResourceFactory();

	static pResourceFactory* _instance;

	//Manager references
	pModelManager* modelManager;
	pMaterialManager* materialManager;
	pImageManager* imageManager;
	pShaderManager* shaderManager;
};

