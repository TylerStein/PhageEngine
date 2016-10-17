#pragma once
#include "pModel.h"
#include "pResourceManager.h"
class ImageManager;
class MaterialManager;

class pModelManager
{
public:
	//This is the type of the handle used for resources
	typedef ResourceHandle<pModel>HandleType;

private:
	ResourceManager<pModel>		modelResources;
	ImageManager*				imageManager;
	MaterialManager*			materialManager;
	static pModelManager*		instance;

	//this is the basic method that returns model or writes an error
	HandleType loadModel(std::string &fileName, std::string &modelName, std::string &resBaseName);

	//this is like the above method but is to be used with Collada type models
	HandleType loadColladaModel(std::string &fileName, std::string &modelName, std::string &resBaseName);

	//empty default constructor
	pModelManager() 
	{
	}

public:
	//singleton
	static pModelManager* getInstance();

	//destructor deletes all resources managed by the manager
	~pModelManager();

	//load the file in by name or handle to return a pointer to the newly loaded object
	HandleType loadFile(std::string &resBaseName, std::string &fileName, std::string &modelName);

	//this gives a model to the manager
	HandleType loadModel(std::string &resName, pModel *model);

	//with this method you can get a pointer to a model if you feed it a handle
	pModel* getModel(HandleType &handle);

	//with this method you can get a pointer to a model if you feed it a name
	pModel* getModel(std::string &nm);

	//if you have the model name you can return a handle
	pModelManager::HandleType pModelManager::getHandle(std::string& resName);

	//this deletes and releaces a model from the manager
	void deleteModel(std::string &nm);

	//this deletes and releaces all models from the manager
	void clear();

	//because pModels have pImages we need to load the manager
	void setImageManager(ImageManager* imgMgr);

	//because pModels have pMaterials we need to load the manager
	void setMaterialManager(MaterialManager* imgMgr);
};

