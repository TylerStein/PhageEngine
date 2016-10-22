#include "pModelManager.h"

pModelManager::pModelManager()
{
}

pModelManager::~pModelManager()
{
	clear();
}

pModelManager::HandleType pModelManager::createModel(char * modelName, char * filePath)
{
	pModelManager::HandleType result(-1);
	
}

pModelManager::HandleType pModelManager::addModel(char * modelName)
{
	return HandleType();
}

pModel * pModelManager::getModel(HandleType & handle)
{
	return nullptr;
}

pModel * pModelManager::getModel(char * modelName)
{
	return nullptr;
}

void pModelManager::deleteModel(char * modelName)
{
}

void pModelManager::clear()
{
}
