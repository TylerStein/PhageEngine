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
	HandleType createMaterial(std::string materialName, std::string filePath);

	//Adds a material to the manager
	HandleType addMaterial(std::string materialName, pMaterial* material);

	//Retreives a material from the manager
	pMaterial* getMaterial(HandleType &handle);
	pMaterial* getMaterial(std::string materialName);

	//Delete a material from the manager
	void deleteMaterial(std::string materialName);

	std::vector<pMaterial*> getAllMaterials();

	//Clear all materials from the manager
	void clear();

private:
	pResourceManager<pMaterial> materialResources;
	//pImageManager* imageManager;
};

