#include "pMaterialManager.h"



pMaterialManager::pMaterialManager()
{
}


pMaterialManager::~pMaterialManager()
{
	clear();
}

void pMaterialManager::setImageManager(pResourceManager<pImage> imgManager)
{
	//imageManager = imgManager
}

pMaterialManager::HandleType pMaterialManager::createMaterial(char * materialName, char * filePath)
{
	pMaterialManager::HandleType result(-1);

	//Loading material from file currently not supported

	return result;
}

pMaterialManager::HandleType pMaterialManager::addMaterial(char * materialName, pMaterial * material)
{
	return materialResources.put((std::string)materialName, material);
}

pMaterial * pMaterialManager::getMaterial(HandleType & handle)
{
	return materialResources.get(handle);
}

pMaterial * pMaterialManager::getMaterial(char * materialName)
{
	pResourceHandle<pMaterial> mtl = materialResources.get((std::string)materialName);
	return materialResources.get(mtl);
}

void pMaterialManager::deleteMaterial(char * materialName)
{
	materialResources.remove((std::string)materialName);
}

void pMaterialManager::clear()
{
	materialResources.clearKeysAndValues();
}
