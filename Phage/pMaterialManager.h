#pragma once
#include "pResourceManager.h"
#include "pMaterial.h"

class pMaterialManager
{
private:
	ResourceManager<pMaterial> materialResources;
	static pMaterialManager* instance;

	//default constructor
	pMaterialManager();
public:
	//This is the type of the handle used for the resources.
	typedef ResourceHandle<pMaterial> HandleType;

	//singleton
	static pMaterialManager* getInstance();

	//destructor deletes all mats from memory
	~pMaterialManager();

	//stores a mat into the pMaterialManager, when a mat is read and needs to be placed into the manager
	HandleType storeMaterial(const pMaterial *mat, const std::string &nm);

	//based on the name a mat is saved under this returns a pointer to that mat
	HandleType getMaterial(const std::string &nm) const;

	//based on the handle a mat is saved under this returns a pointer to that mat
	pMaterial* getMaterial(const HandleType &handle) const;

	//removes the material from the manager but nothing else
	void deleteMaterial(const std::string& resName);
};

