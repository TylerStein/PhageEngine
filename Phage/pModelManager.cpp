#include "pModelManager.h"

void pModelManager::setMaterialManager(pMaterialManager* materialManager)
{
	this->materialManager = materialManager;
}

void pModelManager::setImageManager(pImageManager* imageManager)
{
	this->imageManager = imageManager;
}

pModelManager::pModelManager()
{
}

pModelManager::~pModelManager()
{
	clear();
}


pModelManager::HandleType pModelManager::loadModel(std::string modelName, std::string filePath)
{
	pModelManager::HandleType result(-1);

	if (filePath == "") {
		return result;
	}

	pModel* resModel = pModelLoader::instance()->loadModel(filePath);

	result = modelResources.put(modelName, resModel);

	return result;
}


pModelManager::HandleType pModelManager::addModel(std::string modelName, pModel* mdl)
{
	return modelResources.put(modelName, mdl);
}

pModel * pModelManager::getModel(HandleType & handle)
{
	return modelResources.get(handle);
}

pModel * pModelManager::getModel(std::string modelName)
{
	pResourceHandle<pModel> mdl =  modelResources.get(modelName);
	return modelResources.get(mdl);
}

void pModelManager::deleteModel(std::string modelName)
{
	modelResources.remove(modelName);
}

void pModelManager::clear()
{
	modelResources.clearKeysAndValues();
}
