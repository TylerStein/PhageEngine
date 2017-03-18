#pragma once
#include "pModel.h"
#include "pMaterial.h"
#include "pImage.h"
#include "pResourceManager.h"
#include "pModelManager.h"
#include "pPrimitiveMaker.h"
#include "pShaderManager.h"
#include "pAudioManager.h"
#include "pScriptManager.h"

class pResourceFactory
{
public:
	static pResourceFactory* instance();

	//Functions to set the manager references
	void setModelManager(pModelManager* modelManager);
	void setMaterialManager(pMaterialManager* materialManager);
	void setImageManager(pImageManager* imageManager);
	void setShaderManager(pShaderManager* shaderManager);
	void setAudioManager(pAudioManager* audioManager);
	void setScriptManager(pScriptManager* scriptManager);

	//==MODELS==//
	//Create a model
	pModel* createModel(std::string name, pMaterial* mat, GLfloat* vertPositions, GLfloat* vertNormals, GLfloat* vertColors, GLfloat* vertUVs, GLuint numVerts, GLenum drawMode);
	pModel* createModel(std::string name, pMaterial * material, GLenum drawMode, std::vector<GLfloat> vPositions,
		std::vector<GLuint> vIndeces = std::vector<GLuint>(),
		std::vector<GLfloat> vCoordinates = std::vector<GLfloat>(),
		std::vector<GLfloat> vNormals = std::vector<GLfloat>(),
		std::vector<GLfloat> vTangents = std::vector<GLfloat>(),
		std::vector<GLfloat> vBiTangents = std::vector<GLfloat>(),
		std::vector<GLfloat> vColors = std::vector<GLfloat>());
	//Retreive an existing model from resources
	pModel* getModel(std::string name);
	//Retreive a model from file (returns root model)
	pModel* loadModel(std::string name, std::string path, pMaterial* mat = nullptr);
	//Retreive a model from file, load model scene hierarchy and return as scene nodes
	pSceneNode* loadModelToScene(std::string name, std::string path, pScene& scene, pSceneNode* parent, pMaterial* mat = nullptr);
	//Add a model to the resource manager
	pModel* addModel(std::string name, pModel* model);

	//==MATERIALS==//
	//Create a material
	pMaterial* createMaterial(std::string name, pShader* shader, MaterialInfo info);
	//(UNIMPLEMENTED) Load a material resource file
	pMaterial* loadMaterial(std::string name, std::string path);
	//Retreive an existing material
	pMaterial* getMaterial(std::string name);


	//==SHADERS==//
	//Create a shader
	pShader* createShader(std::string shaderName, attribNameMap attribs, uniformNameMap uniforms, std::string vertShaderPath, std::string fragShaderPath);
	//Retreive existing shader
	pShader* getShader(std::string name);


	//==IMAGES==//
	//Retreive an image from file
	pImage* loadImage(std::string name, std::string path);
	//Retreive an existing image
	pImage* getImage(std::string name);


	//==SCRIPTS==//
	//Retreive an existing script
	pScript* getScript(std::string name);
	//Add a script to the resource pool
	pScript* addScript(std::string name, pScript* script);


	//==SOUNDS==//
	pSoundSystem* createSoundSystem(std::string soundSystemName, std::string audioFilePath, bool loop);
	pSoundSystem* getSoundSystem(std::string name);

	
	//Returns vector of all contained resources
	std::vector<pShader*> getAllShaders();
	std::vector<pModel*> getAllModels();
	std::vector<pMaterial*> getAllMaterials();
	std::vector<pImage*> getAllImages();
	std::vector<pScript*> getAllScripts();

	//==Primitive and Default Creation==
	pModel* createPrimitiveShape(std::string name, pPrimitiveMaker::Primitives prim, glm::vec3 scale = glm::vec3(1), glm::vec3 color = glm::vec3(1), pMaterial* customMaterial = nullptr);
	pSceneNode* addPrimitiveToScene(std::string name, pPrimitiveMaker::Primitives prim, glm::vec3 scale = glm::vec3(1), glm::vec3 color = glm::vec3(1), pMaterial* customMaterial = nullptr);

	//1 pixel white
	pImage* createDebugImage();
	//White unlit material
	pMaterial* createDebugMaterial();
	/*==Debug Shader==
	@Attributes vPosition, vNormal, vCoordinate, vColor
	@Uniforms	cameraView, projectionView, modelView, diffuseColor
	*/
	pShader* createDebugShader();
	/*==Skybox Shader==
	@Attributes vPosition
	@Uniforms	transformMatrix, fCubeMapTexture
	*/
	pShader* createSkyboxShader();
	//Lit phong shader
	pShader* createPhongShader();

private:
	pResourceFactory();
	~pResourceFactory();

	static pResourceFactory* _instance;

	//Manager references
	pModelManager* modelManager;
	pMaterialManager* materialManager;
	pImageManager* imageManager;
	pShaderManager* shaderManager;
	pAudioManager* soundSystemManager;
	pScriptManager* scriptManager;
};

