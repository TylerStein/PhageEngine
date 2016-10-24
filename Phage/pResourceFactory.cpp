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

pModel * pResourceFactory::createModel(std::string name, pMaterial * mat, GLfloat * vertPositions,  GLfloat * vertNormals, GLfloat * vertColors, GLfloat * vertUVs, GLuint numVerts)
{
	//Create the model in memory
	pModel* mdl = new pModel((char*)name.c_str(), mat, vertPositions, vertNormals, vertColors, vertUVs, numVerts);
	//Add the model to the manager table
	pResourceHandle<pModel> mdlH = modelManager->addModel((char*)name.c_str(), mdl);
	//Return the model from the table
	return modelManager->getModel(mdlH);
}

pMaterial * pResourceFactory::createMaterial(std::string name, pMaterial::materialInfo info)
{
	//Create the material in memory
	pMaterial* mtl = new pMaterial(name, info.diffuse, info.ambient, info.shininess, info.alpha);
	//Add the material to the manager table
	pResourceHandle<pMaterial> mtlH = materialManager->addMaterial((char*)name.c_str(), mtl);
	//Return the material from the table
	return materialManager->getMaterial(mtlH);
}

pMaterial * pResourceFactory::createMaterial(std::string name, pImage * img)
{
	//Create the material in memory
	pMaterial* mtl = new pMaterial(name.c_str(), img);
	//Add the material to the manager table
	pResourceHandle<pMaterial> mtlH = materialManager->addMaterial((char*)name.c_str(), mtl);
	//Return the material from the table
	return materialManager->getMaterial(mtlH);
}

pMaterial * pResourceFactory::createMaterial(std::string name, std::vector<pImage*> images, GLenum blendMode)
{
	//Create the material in memory
	pMaterial* mtl = new pMaterial(name.c_str(), images, blendMode);
	//Add the material to the manager table
	pResourceHandle<pMaterial> mtlH = materialManager->addMaterial((char*)name.c_str(), mtl);
	//Return the material from the table
	return materialManager->getMaterial(mtlH);
}

pImage * pResourceFactory::createImage(std::string name, std::string filePath)
{
	//Create the image in memory
	pImage* img = new pImage((char*)name.c_str(), (char*)filePath.c_str());
	//Add the image to the manager table
	pResourceHandle<pImage> imgH = imageManager->addImage((char*)name.c_str(), img);
	//Return the image from the table
	return imageManager->getImage(imgH);
}

pModel * pResourceFactory::getModel(char* name, char* path)
{
	//Generate a model and get it's handle
	pResourceHandle<pModel> mdlHandle = modelManager->createModel(name, path);
	//Get the model from the hashtable
	return modelManager->getModel(mdlHandle);
}

pModel * pResourceFactory::getModel(char * name)
{
	//Get the model from the manager's hashtable
	return modelManager->getModel(name);

}

pMaterial * pResourceFactory::getMaterial(char * name, char * path)
{
	pResourceHandle<pMaterial> mtlH = materialManager->createMaterial(name, path);
	return materialManager->getMaterial(mtlH);
}

pMaterial * pResourceFactory::getMaterial(char * name)
{
	return materialManager->getMaterial(name);
}

pImage * pResourceFactory::getImage(char * name, char * path)
{
	pResourceHandle<pImage> imgH = imageManager->createImage(name, path);
	return imageManager->getImage(imgH);
}

pImage * pResourceFactory::getImage(char * name)
{
	return imageManager->getImage(name);
}
