#include "pScene.h"



pScene::pScene()
{
	initialize();
}


pScene::~pScene()
{
}

void pScene::initialize()
{
	sceneGraph = new pSceneGraph();
}

pSceneObject* pScene::createNewObject(pModel* model, pScript* script, pSoundSystem* soundSystem, pCamera* camera, glm::vec3 & pos, glm::vec3 & rot, glm::vec3 & scaling, std::string objName, pSceneNode* parent)
{
	pSceneObject* newObject = new pSceneObject(model, script, soundSystem, camera);
	if (parent == nullptr)
	{
		pSceneNode* newNode = new pSceneNode(pos, rot, scaling, objName, newObject, sceneGraph->getRootSceneNode());
		newObject->node = newNode;
	}
	else
	{
		pSceneNode* newNode = new pSceneNode(pos, rot, scaling, objName, newObject, parent);
		newObject->node = newNode;
	}
	return newObject;
}