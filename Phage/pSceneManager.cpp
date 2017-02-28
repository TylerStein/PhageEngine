#include "pSceneManager.h"


pSceneManager* pSceneManager::_instance = 0;
pSceneManager::pSceneManager()
{

}

pSceneManager::~pSceneManager()
{

}

pSceneManager * pSceneManager::instance()
{
	if (_instance == 0)
	{
		_instance = new pSceneManager();
	}
	return _instance;
}

void pSceneManager::updateScene(double deltaTime)
{

}

void pSceneManager::renderScene(pRenderer * renderer)
{
	currentScene->sceneGraph->renderSceneGraph(renderer, currentScene->sceneGraph->getRootSceneNode());
}

void pSceneManager::addScene()
{
	pScene* scene = new pScene();
	sceneList.push_back(scene);
}
