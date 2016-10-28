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

pMaterialManager::HandleType pMaterialManager::createMaterial(std::string materialName, std::string filePath)
{
	pMaterialManager::HandleType result(-1);

	//Loading material from file currently not supported

	return result;
}

pMaterialManager::HandleType pMaterialManager::addMaterial(std::string materialName, pMaterial * material)
{
	return materialResources.put(materialName, material);
}

pMaterial * pMaterialManager::getMaterial(HandleType & handle)
{
	return materialResources.get(handle);
}

pMaterial * pMaterialManager::getMaterial(std::string materialName)
{
	pResourceHandle<pMaterial> mtl = materialResources.get(materialName);
	return materialResources.get(mtl);
}

void pMaterialManager::deleteMaterial(std::string materialName)
{
	materialResources.remove(materialName);
}

void pMaterialManager::clear()
{
	materialResources.clearKeysAndValues();
}
