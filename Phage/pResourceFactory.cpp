#include "pResourceFactory.h"



pResourceFactory::pResourceFactory()
{
}


pResourceFactory::~pResourceFactory()
{
	delete modelManager;
	delete materialManager;
	delete imageManager;
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

pModel * pResourceFactory::createModel(std::string name, pMaterial * mat, GLfloat * vertPositions,  GLfloat * vertNormals, GLfloat * vertColors, GLfloat * vertUVs, GLuint numVerts, GLenum drawMode)
{
	//Create the model in memory
	//pModel* mdl = new pModel(name, mat, vertPositions, vertNormals, vertColors, vertUVs, numVerts);
	pModel* mdl = new pModel(name, mat, vertPositions, vertColors, vertUVs, vertNormals, numVerts, drawMode);
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

pImage * pResourceFactory::createDebugImage(std::string name)
{
	//Create the image in memory
	pImage* img = new pImage(name);
	//Add the image to the manager table
	pResourceHandle<pImage> imgH = imageManager->addImage(name, img);
	//Return the image from the table
	return imageManager->getImage(imgH);
}

pShader * pResourceFactory::createShader(std::string name, std::string vertShaderPath, std::string fragShaderPath, GLint flags)
{
	pResourceHandle<pShader> shdr = pResourceHandle<pShader>(-1);
	shdr = shaderManager->createShader(name, vertShaderPath, fragShaderPath, flags);
	return shaderManager->getShader(shdr);
}

pModel * pResourceFactory::getModel(std::string name, std::string path)
{
	pResourceHandle<pShader> mdl = pResourceHandle<pShader>(-1);
	//mdl = createModel
	std::cout << "Attempted to read model from file, not yet functional!" << std::endl;
	return nullptr;
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
