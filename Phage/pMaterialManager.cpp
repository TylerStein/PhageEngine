#include "pMaterialManager.h"
#include "LogManager.h"

//singleton
pMaterialManager* pMaterialManager::instance = nullptr;

//singleton constructor
pMaterialManager * pMaterialManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new pMaterialManager();
	}
	return instance;
}

//destructor
pMaterialManager::~pMaterialManager()
{
	materialResources.clearKeysAndValues();
}

//stores a mat into the pMaterialManager, when a mat is read and needs to be placed into the manager
pMaterialManager::HandleType pMaterialManager::storeMaterial(const pMaterial * mat, const std::string & nm)
{
	pMaterialManager::HandleType		result(-1);

	// check if the resource name is already in use
	result = materialResources.get(nm);
	if (!result.isNull()) {
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Name is in use") + (std::string&)nm);
	}

	result = materialResources.put((std::string&)nm, (pMaterial*)mat);
	LogManager::instance()->log(LogManager::LOG_INFO,std::string("Material manager: stored material ") + nm);
	return result;
}

//based on the name a mat is saved under this returns a pointer to that mat
pMaterialManager::HandleType pMaterialManager::getMaterial(const std::string & nm) const
{
	pMaterialManager::HandleType result(-1);
	result = materialResources.get(nm);

	return result;
	
}

//based on the handle a mat is saved under this returns a pointer to that mat
pMaterial * pMaterialManager::getMaterial(const pMaterialManager::HandleType & handle) const
{
	pMaterial *result = materialResources.get((pMaterialManager::HandleType) handle);

	if (result == NULL)
	{
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Invalid Handle", handle.getIndex()));
	}

	return result;
}

//removes the material from the manager but nothing else
void pMaterialManager::deleteMaterial(const std::string & resName)
{
	HandleType result = materialResources.get(resName);
	if (!result.isNull()) {
		LogManager::instance()->log(LogManager::LOG_ERROR, std::string("Material not found " + resName, -1));
	}
	materialResources.remove((std::string&)resName);
}
