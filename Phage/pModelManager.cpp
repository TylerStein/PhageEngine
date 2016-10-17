#include "pModelManager.h"
#include <vector>
#include "LogManager.h"
//#include "AssimpModelLoader.h"

//singleton
pModelManager* pModelManager::instance = nullptr;
//constructor
pModelManager* pModelManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new pModelManager();
	}
	return instance;
}

//Destructor runs clear()
pModelManager::~pModelManager()
{
	clear();
}

//load the file in by name or handle to return a pointer to the newly loaded object
pModelManager::HandleType pModelManager::loadFile(std::string &resBaseName, std::string &fileName,
	std::string &modelName)
{
	pModelManager::HandleType		result(-1);

	result = loadModel(fileName, modelName, resBaseName);
	return result;
}

//if you have the model name you can return a handle
pModelManager::HandleType pModelManager::getHandle(std::string& resName)
{
	pModelManager::HandleType		result(-1);
	result = modelResources.get(resName);
	return result;
}

//this gives a model to the manager or writes an error
pModelManager::HandleType pModelManager::loadModel(std::string &resName, pModel *model)
{
	pModelManager::HandleType		result(-1);
	// check if the resource name is already in use
	result = modelResources.get(resName);
	if (!result.isNull()) {
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Name in use " + resName));
	}

	result = modelResources.put(resName, model);
	return result;
}

//with this method you can get a pointer to a model if you feed it a handle
pModel* pModelManager::getModel(pModelManager::HandleType &handle)
{
	pModel *result = modelResources.get(handle);
	if (result == NULL)
	{
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Invalid handle ", handle.getIndex()));
	}
	return result;
}

//with this method you can get a pointer to a model if you feed it a name
pModel* pModelManager::getModel(std::string &nm)
{
	pModel *result = NULL;
	pModelManager::HandleType handle = modelResources.get(nm);
	if (handle.isNull())
	{
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Model name unknown: ") + nm);
	}
	result = getModel(handle);
	return result;
}
//this deletes and releaces a model from the manager
void pModelManager::deleteModel(std::string &nm)
{
	pModel *model = getModel(nm);
	modelResources.remove(nm);
	delete model;
}

//this deletes and releaces all models from the manager
void pModelManager::clear()
{
	modelResources.clearKeysAndValues();
}

//because pModels have pImages we need to load the manager
void pModelManager::setImageManager(ImageManager* imgMgr)
{
	imageManager = imgMgr;
}

//because pModels have pMaterials we need to load the manager
void pModelManager::setMaterialManager(MaterialManager* matMgr)
{
	materialManager = matMgr;
}

//private constructors
/*
//this is the basic method that returns model or writes an error
pModelManager::HandleType pModelManager::loadModel(std::string &fileName,
std::string &modelName,
std::string &resBaseName)
{
AssimpModelLoader loader;
pModel* mdl = loader.loadModel(*imageManager, *materialManager, *this, resBaseName, fileName);

HandleType result = getHandle(resBaseName);

return result;
}

//this is like the above method but is to be used with Collada type models
pModelManager::HandleType pModelManager::loadColladaModel(std::string &fileName,
std::string &modelName,
std::string &resBaseName)
{
HandleType result;
ColladaModelLoader loader;

loader.loadFile(fileName);
SimpleModel *smodel = loader.getGeometryModel(modelName);
std::string simpleModelName = resBaseName + std::string("Model");
result = loadModel(simpleModelName, smodel);
return result;

}
*/
