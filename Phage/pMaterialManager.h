#pragma once
#include "pResourceManager.h"
#include "pMaterial.h"

class pMaterialManager
{
public:
	typedef pResourceHandle<pMaterial> HandleType;

	pMaterialManager();
	~pMaterialManager();

	void setImageManager(pResourceManager<pImage> imgManager);

	//Gets a material from file on disk
	HandleType createMaterial(char* materialName, char* filePath);

	//Adds a material to the manager
	HandleType addMaterial(char* materialName, pMaterial* material);

	//Retreives a material from the manager
	pMaterial* getMaterial(HandleType &handle);
	pMaterial* getMaterial(char* materialName);

	//Delete a material from the manager
	void deleteMaterial(char* materialName);

	//Clear all materials from the manager
	void clear();

private:
	pResourceManager<pMaterial> materialResources;
	//pImageManager* imageManager;
};

