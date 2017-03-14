#include "pResourceFactory.h"
#include "DefaultPaths.h"
#include "pScene.h"

pResourceFactory* pResourceFactory::_instance = 0;
pResourceFactory::pResourceFactory()
{
}


pResourceFactory::~pResourceFactory()
{
	delete modelManager;
	delete materialManager;
	delete imageManager;
	delete shaderManager;
}

pResourceFactory * pResourceFactory::instance()
{
	if (_instance == 0) {
		_instance = new pResourceFactory();
	}
	return _instance;
}

void pResourceFactory::setModelManager(pModelManager* modelManager)
{
	this->modelManager = modelManager;
}

void pResourceFactory::setMaterialManager(pMaterialManager* materialManager)
{
	this->materialManager = materialManager;
}

void pResourceFactory::setImageManager(pImageManager* imageManager)
{
	this->imageManager = imageManager;
}

void pResourceFactory::setShaderManager(pShaderManager * shaderManager)
{
	this->shaderManager = shaderManager;
}

void pResourceFactory::setAudioManager(pAudioManager * audioManager)
{
	this->soundSystemManager = audioManager;
}

void pResourceFactory::setScriptManager(pScriptManager * scriptManager)
{
	this->scriptManager = scriptManager;
}

pModel * pResourceFactory::createModel(std::string name, pMaterial * mat, GLfloat * vertPositions,  GLfloat * vertNormals, GLfloat * vertColors, GLfloat * vertUVs, GLuint numVerts, GLenum drawMode)
{
	//Create the model in memory
	pModel* mdl = new pModel(name, mat, drawMode, numVerts, vertPositions, 0, nullptr, vertUVs, vertNormals, nullptr, nullptr, vertColors);
	//Add the model to the manager table
	pResourceHandle<pModel> mdlH = modelManager->addModel(name, mdl);
	//Return the model from the table
	return modelManager->getModel(mdlH);
}

pModel * pResourceFactory::createModel(std::string name, pMaterial * material, GLenum drawMode, std::vector<GLfloat> vPositions, std::vector<GLuint> vIndeces, std::vector<GLfloat> vCoordinates, std::vector<GLfloat> vNormals, std::vector<GLfloat> vTangents, std::vector<GLfloat> vBiTangents, std::vector<GLfloat> vColors)
{
	//Create the model in memory
	pModel* mdl = new pModel(name, material, drawMode, vPositions, vIndeces, vCoordinates, vNormals, vTangents, vBiTangents, vColors);
	//Add the model to the manager table
	pResourceHandle<pModel> mdlH = modelManager->addModel(name, mdl);
	//Return the model from the table
	return modelManager->getModel(mdlH);
}


pMaterial * pResourceFactory::createMaterial(std::string name, pShader* shader, MaterialInfo info)
{
	//Create the material in memory
	pMaterial* mtl = new pMaterial(name, shader, info);
	//Add the material to the manager table
	pResourceHandle<pMaterial> mtlH = materialManager->addMaterial(name, mtl);
	//Return the material from the table
	return materialManager->getMaterial(mtlH);
}


pImage * pResourceFactory::createImage(std::string name, std::string filePath)
{
	//Create the image in memory
	pImage* img = new pImage(name, filePath);
	//Add the image to the manager table
	pResourceHandle<pImage> imgH = imageManager->addImage(name, img);
	//Return the image from the table
	return imageManager->getImage(imgH);
}

pModel * pResourceFactory::createPrimitiveShape(std::string name, pPrimitiveMaker::Primitives prim, glm::vec3 scale, glm::vec3 color, pMaterial* customMaterial)
{
	pModel* tmpModel = pPrimitiveMaker::instance()->GetPrimitive(name, prim, scale, customMaterial, color);
	
	pResourceHandle<pModel> mdlH = modelManager->addModel(name, tmpModel);

	return modelManager->getModel(mdlH);
}

pImage * pResourceFactory::createDebugImage()
{
	//Check for the image already existing
	pImage* tmpImg = imageManager->getImage("DEBUG_IMAGE");

	//If it doesn't exist, make a new one
	if (tmpImg == nullptr) {
		tmpImg = new pImage("DEBUG_IMAGE");
		pResourceHandle<pImage> imgH = imageManager->addImage("DEBUG_IMAGE", tmpImg);
	}

	//Return the image
	return tmpImg;
}

pMaterial * pResourceFactory::createDebugMaterial()
{
	pMaterial* tmpMat = materialManager->getMaterial("Primitive");

	if (tmpMat == nullptr) {
		MaterialInfo matInfo;
		matInfo.reset();
		pResourceHandle<pMaterial> mtlH = materialManager->addMaterial("Primitive", new pMaterial("Primitive", createDebugShader(), matInfo));
		return materialManager->getMaterial(mtlH);
	}
}

pShader * pResourceFactory::createDebugShader()
{
	pShader* tmpShader = shaderManager->getShader("Primitive");
	
	//Shader does not already exist
	if (tmpShader == nullptr) {
#ifdef _DEBUG
		std::cout << "Primitive shader loaded for first time" << std::endl;
#endif
		attribNameMap atrMap = attribNameMap();
		uniformNameMap uniMap = uniformNameMap();
		{
			atrMap.insert(Attributes::VertexPosition, "vPosition");
			atrMap.insert(Attributes::VertexCoordinate, "vTexCoord");
			atrMap.insert(Attributes::VertexNormal, "vNormal");
			atrMap.insert(Attributes::VertexColor, "vColor");

			uniMap.insert(Uniforms::Camera_View, "cameraView");
			uniMap.insert(Uniforms::Projection_View, "projectionView");
			uniMap.insert(Uniforms::Model_View, "modelView");

			uniMap.insert(Uniforms::DiffuseColor, "diffuseColor");
		}
		pResourceHandle<pShader> shdrH = shaderManager->addShader("Primitive", new pShader("Primitive", atrMap, uniMap, shader_primitive_vert, shader_primitive_frag));
		return shaderManager->getShader(shdrH);
	}

#ifdef _DEBUG
		std::cout << "Primitive shader already loaded" << std::endl;
#endif
	return tmpShader;
}

pShader * pResourceFactory::createSkyboxShader()
{
	pShader* tmpShader = shaderManager->getShader("Skybox");

	//Shader does not already exist
	if (tmpShader == nullptr) {
#ifdef _DEBUG
		std::cout << "Skybox shader loaded for first time" << std::endl;
#endif
		attribNameMap atrMap = attribNameMap();
		uniformNameMap uniMap = uniformNameMap();
		{
			atrMap.insert(Attributes::VertexPosition, "vPosition");
			uniMap.insert(Uniforms::ModelViewProjection, "transformMatrix");
			uniMap.insert(Uniforms::Cube_Map_Texture, "fCubeMapTexture");
		}
		pResourceHandle<pShader> shdrH = shaderManager->addShader("Skybox", new pShader("Skybox", atrMap, uniMap, shader_skybox_vert, shader_skybox_frag));
		return shaderManager->getShader(shdrH);
	}

#ifdef _DEBUG
	std::cout << "Skybox shader already loaded" << std::endl;
#endif
	return tmpShader;
}

pShader * pResourceFactory::createPhongShader()
{
	pShader* tmpShader = shaderManager->getShader("DEFAULT_PHONG");

	//Shader does not already exist
	if (tmpShader == nullptr) {
#ifdef _DEBUG
		std::cout << "Phong shader loaded for first time" << std::endl;
#endif
		attribNameMap atrMap = attribNameMap();
		uniformNameMap uniMap = uniformNameMap();
		{
			atrMap.insert(Attributes::VertexPosition, "vPosition");
			atrMap.insert(Attributes::VertexCoordinate, "vTexCoord");
			atrMap.insert(Attributes::VertexNormal, "vNormal");

			uniMap.insert(Uniforms::Projection_View, "projView");
			uniMap.insert(Uniforms::Model_View, "modelView");
			uniMap.insert(Uniforms::Camera_View, "cameraView");
			uniMap.insert(Uniforms::Normal_View, "normalView");

			uniMap.insert(Uniforms::Camera_Position, "cameraLocation");

			uniMap.insert(Uniforms::DiffuseColor, "fDiffuse");
			uniMap.insert(Uniforms::AmbientColor, "fAmbient");
			uniMap.insert(Uniforms::DiffuseTexture, "fTexture");
			uniMap.insert(Uniforms::SpecularTexture, "fSpecularMap");
			uniMap.insert(Uniforms::SpecularColor, "fSpecular");
			uniMap.insert(Uniforms::Shininess, "fShininess");

			uniMap.insert(Uniforms::Light_Count, "lightCount");
			uniMap.insert(Uniforms::Light_Type, "lType");
			uniMap.insert(Uniforms::Light_Range, "lRange");
			uniMap.insert(Uniforms::Light_Position, "lPosition");
			uniMap.insert(Uniforms::Light_Angle, "lAngle");
			uniMap.insert(Uniforms::Light_Cone, "lCone");
			uniMap.insert(Uniforms::Light_Color, "lColor");
			uniMap.insert(Uniforms::Light_Power, "lPower");
			uniMap.insert(Uniforms::Light_Ambient, "lAmbient");
		}
		pResourceHandle<pShader> shdrH = shaderManager->addShader("DEFAULT_PHONG", new pShader("DEFAULT_PHONG", atrMap, uniMap, shader_phong_vert, shader_phong_frag));
		return shaderManager->getShader(shdrH);
	}

#ifdef _DEBUG
	std::cout << "Phong shader already loaded" << std::endl;
#endif
	return tmpShader;
}

pSoundSystem * pResourceFactory::createSoundSystem(std::string soundSystemName, std::string audioFilePath, bool loop)
{
	pResourceHandle<pSoundSystem> sndSystm = soundSystemManager->createSoundSystem(soundSystemName, audioFilePath, loop);
	return soundSystemManager->getSoundSystem(sndSystm);
}

pSoundSystem * pResourceFactory::getSoundSystem(std::string name)
{
	return soundSystemManager->getSoundSystem(name);
}

pShader * pResourceFactory::createShader(std::string shaderName, attribNameMap attribs, uniformNameMap uniforms, std::string vertShaderPath, std::string fragShaderPath)
{
	pResourceHandle<pShader> shdr = pResourceHandle<pShader>(-1);
	shdr = shaderManager->createShader(shaderName, attribs, uniforms, vertShaderPath, fragShaderPath);
	return shaderManager->getShader(shdr);
}

pModel * pResourceFactory::loadModel(std::string name, std::string path, pMaterial* mat)
{
	pResourceHandle<pModel> mdl = pResourceHandle<pModel>(-1);
	
	mdl = modelManager->loadModel(name, path, mat);

	return modelManager->getModel(mdl);
}

pSceneNode * pResourceFactory::loadModelToScene(std::string name, std::string path, pScene & scene, pSceneNode * parent, pMaterial * mat)
{
	pSceneNode* resNode = pModelLoader::instance()->loadModelToSceneObjects(path, mat);

	scene.addExistingNodes(resNode, parent);

	return resNode;
}

pModel * pResourceFactory::getModel(std::string name)
{
	//Get the model from the manager's hashtable
	return modelManager->getModel(name);

}

pMaterial * pResourceFactory::getMaterial(std::string name, std::string path)
{
	pResourceHandle<pMaterial> mtlH = materialManager->createMaterial(name, path);
	return materialManager->getMaterial(mtlH);
}

pMaterial * pResourceFactory::getMaterial(std::string name)
{
	return materialManager->getMaterial(name);
}

pImage * pResourceFactory::getImage(std::string name, std::string path)
{
	pResourceHandle<pImage> imgH = imageManager->createImage(name, path);
	return imageManager->getImage(imgH);
}

pImage * pResourceFactory::getImage(std::string name)
{
	return imageManager->getImage(name);
}

pShader * pResourceFactory::getShader(std::string name)
{
	return shaderManager->getShader(name);
}

pScript * pResourceFactory::getScript(std::string name)
{
	return scriptManager->getScript(name);
}

pScript* pResourceFactory::addScript(std::string name, pScript * script)
{
	script->OnCreate();
	return scriptManager->addScript(name, script);
}

std::vector<pShader*> pResourceFactory::getAllShaders()
{
	return shaderManager->getAllShaders();
}

std::vector<pModel*> pResourceFactory::getAllModels()
{
	return modelManager->getAllModels();
}

std::vector<pMaterial*> pResourceFactory::getAllMaterials()
{
	return materialManager->getAllMaterials();
}

std::vector<pImage*> pResourceFactory::getAllImages()
{
	return imageManager->getAllImages();
}

std::vector<pScript*> pResourceFactory::getAllScripts()
{
	return std::vector<pScript*>();
}
