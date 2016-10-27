#pragma once
#include "pResourceManager.h"
#include "pMaterialManager.h"
#include "pImageManager.h"
#include "pModelLoader.h"
#include "pModel.h"

class pModelManager
{
public:
	typedef pResourceHandle<pModel> HandleType;

	//Functions to take manager references
	void setMaterialManager(pMaterialManager* materialManager);
	void setImageManager(pImageManager* imageManager);

	pModelManager();
	~pModelManager();

	//Gets a model from file on disk
	HandleType createModel(std::string modelName, std::string filePath);

	//Adds a model to the manager
	HandleType addModel(std::string modelName, pModel* mdl);

	//Retreives a model from the manager
	pModel* getModel(HandleType &handle);
	pModel* getModel(std::string modelName);

	//Delete a model from the manager
	void deleteModel(std::string modelName);

	//Clear all models from the manager
	void clear();
	
private:
	pResourceManager<pModel> modelResources;
	pMaterialManager* materialManager;
	pImageManager* imageManager;


};

