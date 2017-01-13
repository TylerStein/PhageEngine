#include "pResourceFactory.h"
#include "DefaultPaths.h"

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
	delete soundSystemManager;
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


pModel * pResourceFactory::createModel(std::string dir, pShader *shader, GLenum drawMode)
{
	ModelInfo info = pModelLoader::instance()->loadModel(dir, shader);

	std::string name = "name";
	GLfloat *tVerts;
	GLfloat *tCVerts;
	GLfloat *tNormals;
	GLfloat *uvVerts;

	tVerts = (GLfloat*)malloc(sizeof(GLfloat) * info.positions.size());
	tCVerts = (GLfloat*)malloc(sizeof(GLfloat) * info.colors.size());
	tNormals = (GLfloat*)malloc(sizeof(GLfloat) * info.normals.size());
	uvVerts = (GLfloat*)malloc(sizeof(GLfloat) * info.uvs.size());
	
	
	for (int i = 0; i < info.positions.size(); i++) {
		tVerts[i] = info.positions[i];
		tNormals[i] = info.normals[i];
	}
	for (int i = 0; i < info.colors.size(); i++)
	{
		tCVerts[i] = info.colors[i];
	}
	for (int i = 0; i < info.uvs.size(); i++) {
		uvVerts[i] = info.uvs[i];
	}

	pModel* mdl = new pModel((char*)name.c_str(), info.mat, tVerts, tCVerts, uvVerts, tNormals, info.numVerts, drawMode);

	free(tVerts);
	free(tCVerts);
	free(tNormals);
	free(uvVerts);
	pResourceHandle<pModel> mdlH = modelManager->addModel(name, mdl);

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
	pModel* tmpModel = pPrimitiveMaker::instance()->GetPrimitive(name, prim, scale, color);
	if (customMaterial != nullptr) { tmpModel->setMaterial(customMaterial); }
	
	pResourceHandle<pModel> mdlH = modelManager->addModel(name, tmpModel);

	return modelManager->getModel(mdlH);
}

pImage * pResourceFactory::createDebugImage(std::string name)
{
	//Create the image in memory
	pImage* img = new pImage(name);
	//Add the image to the manager table
	pResourceHandle<pImage> imgH = imageManager->addImage(name, img);
	//Return the image from the table
	return imageManager->getImage(imgH);
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

pShader * pResourceFactory::createShader(std::string shaderName, attribNameMap attribs, uniformNameMap uniforms, std::string vertShaderPath, std::string fragShaderPath)
{
	pResourceHandle<pShader> shdr = pResourceHandle<pShader>(-1);
	shdr = shaderManager->createShader(shaderName, attribs, uniforms, vertShaderPath, fragShaderPath);
	return shaderManager->getShader(shdr);
}

pModel * pResourceFactory::loadModel(std::string name, std::string path, pMaterial* mat)

pSoundSystem * pResourceFactory::createSoundSystem(std::string soundSystemName, std::string audioFilePath, bool loop)
{
	pResourceHandle<pSoundSystem> sndSystm = soundSystemManager->createSoundSystem(soundSystemName, audioFilePath, loop);
	return soundSystemManager->getSoundSystem(sndSystm);
}

pModel * pResourceFactory::getModel(std::string name, std::string path)
{
	pResourceHandle<pModel> mdl = pResourceHandle<pModel>(-1);
	
	mdl = modelManager->loadModel(name, path, mat);

	return modelManager->getModel(mdl);
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

pSoundSystem * pResourceFactory::getSoundSystem(std::string name)
{
	return soundSystemManager->getSoundSystem(name);
}


