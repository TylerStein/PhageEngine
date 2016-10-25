#include "pModelManager.h"

void pModelManager::setMaterialManager(pMaterialManager* materialManager)
{
	this->materialManager = materialManager;
}

void pModelManager::setImageManager(pImageManager* imageManager)
{
	this->imageManager = imageManager;
}

pModelManager::pModelManager()
{
}

pModelManager::~pModelManager()
{
	clear();
}

pModelManager::HandleType pModelManager::createModel(std::string modelName, std::string filePath)
{
	pModelManager::HandleType result(-1);

	//Load the model info from file
	pModelLoader::ModelInfo info = pModelLoader::instance()->loadModel(filePath);

	//Placeholder colors
	const int sz = info.positions.size();
	GLfloat* vertColors = (GLfloat*)malloc(info.positions.size() * 3 * sizeof(GLfloat));
	for (int x(0); x < info.positions.size(); ++x) {
		vertColors[x] = 1.0f;
	}

	//Get positions
	GLfloat* vertPositions = (GLfloat*)malloc(info.positions.size() * 3 * sizeof(GLfloat));
	int y = 0;
	for (int x(0); x < info.positions.size(); x+=3) {
		y++;
		vertPositions[x] = info.positions[y].x;
		vertPositions[x + 1] = info.positions[y].y;
		vertPositions[x + 2] = info.positions[y].z;
	}

	//Get normals
	GLfloat* vertNormals = (GLfloat*)malloc(info.normals.size() * 3 * sizeof(GLfloat));
	y = 0;
	for (int x(0); x < info.positions.size(); x += 3) {
		y++;
		vertNormals[x] = info.normals[y].x;
		vertNormals[x + 1] = info.normals[y].y;
		vertNormals[x + 2] = info.normals[y].z;
	}

	//Get UVs
	GLfloat* vertUVs = (GLfloat*)malloc(info.uvs.size() * 3 * sizeof(GLfloat));
	y = 0;
	for (int x(0); x < info.positions.size(); x += 3) {
		y++;
		vertUVs[x] = info.uvs[y].x;
		vertUVs[x + 1] = info.uvs[y].y;
		vertUVs[x + 2] = info.uvs[y].z;
	}

	//Check if there's an associated material

	//Generate or grab the material
	pMaterial* mat = new pMaterial("DefaultMat");

	//Create a model based on that info
	pModel* mdl = new pModel(modelName, mat, vertPositions, vertNormals, vertColors, vertUVs, (GLuint)info.positions.size());
	
	result = modelResources.put(modelName, mdl);

	//Delete vert information, the model constructor copies this info
	delete[] vertPositions;
	delete[] vertNormals;
	delete[] vertColors;
	delete[] vertUVs;

	return result;
}

pModelManager::HandleType pModelManager::addModel(std::string modelName, pModel* mdl)
{
	return modelResources.put(modelName, mdl);
}

pModel * pModelManager::getModel(HandleType & handle)
{
	return modelResources.get(handle);
}

pModel * pModelManager::getModel(std::string modelName)
{
	pResourceHandle<pModel> mdl =  modelResources.get(modelName);
	return modelResources.get(mdl);
}

void pModelManager::deleteModel(std::string modelName)
{
	modelResources.remove(modelName);
}

void pModelManager::clear()
{
	modelResources.clearKeysAndValues();
}
