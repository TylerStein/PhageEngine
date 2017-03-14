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


pModelManager::HandleType pModelManager::loadModel(std::string modelName, std::string filePath, pMaterial* mat)
{
	pModelManager::HandleType result(-1);

	if (filePath == "") {
		return result;
	}

	pModel* resModel = pModelLoader::instance()->loadModel(filePath, mat);

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

std::vector<pModel*> pModelManager::getAllModels()
{
	auto inList = modelResources.getAll();
	std::vector<pModel*> outList = std::vector<pModel*>();
	for (int i = 0; i < inList.size(); ++i) {
		outList.push_back(
			modelResources.get(
				inList.at(i)
			));
	}

	return outList;
}

void pModelManager::deleteModel(std::string modelName)
{
	modelResources.remove(modelName);
}

void pModelManager::clear()
{
	modelResources.clearKeysAndValues();
}
