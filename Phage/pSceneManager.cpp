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

void pSceneManager::startScene()
{
	if (hasCurrentScene()) {
	
	}
}

void pSceneManager::updateScene(double deltaTime)
{
	if (hasCurrentScene()) {
		currentScene->sceneGraph->updateSceneGraph(deltaTime, currentScene->sceneGraph->getRootSceneNode());
	}
}

void pSceneManager::renderScene(pRenderer * renderer)
{
	if (hasCurrentScene()) {
		currentScene->sceneGraph->renderSceneGraph(renderer, currentScene->sceneGraph->getRootSceneNode());
	}
}

void pSceneManager::endScene()
{
	if (hasCurrentScene()) {

	}
}

void pSceneManager::addScene()
{
	pScene* scene = new pScene();
	sceneList.push_back(scene);
	if (currentScene == nullptr) {
		currentScene = scene;
	}
}
