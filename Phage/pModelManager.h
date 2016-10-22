#pragma once
#include "pResourceManager.h"
#include "pModel.h"

class pModelManager
{
public:
	typedef pResourceHandle<pModel> HandleType;

	static pModelManager* Instance() {
		if (instance == nullptr) { instance = new pModelManager(); }
		return instance;
	}



	pModelManager();
	~pModelManager();
	
private:
	static pModelManager* instance;
	pResourceManager<pModel> modelResources;

	//Gets a model from file on disk
	HandleType createModel(char* modelName, char* filePath);

	//Adds a model to the manager
	HandleType addModel(char* modelName);

	//Retreives a model from the manager
	pModel* getModel(HandleType &handle);
	pModel* getModel(char* modelName);

	//Delete a model from the manager
	void deleteModel(char* modelName);

	//Clear all models from the manager
	void clear();

};

