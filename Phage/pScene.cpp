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

void pScene::createNewObject(pModel* model, pScript* script, pSoundSystem* soundSystem, pCamera* camera, glm::vec3 & pos, glm::vec3 & rot, GLfloat amount, glm::vec3 & scaling, std::string objName, pSceneNode* parent)
{
	pSceneObject* newObject = new pSceneObject(model, script, soundSystem, camera);
	if (parent == nullptr)
	{
		pSceneNode* newNode = new pSceneNode(pos, rot, amount, scaling, objName, newObject, sceneGraph->getRootSceneNode());
	}
	else
	{
		pSceneNode* newNode = new pSceneNode(pos, rot, amount, scaling, objName, newObject, parent);
	}
}