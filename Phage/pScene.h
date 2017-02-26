#pragma once

#include "pSceneGraph.h"
#include "pSceneNode.h"
#include "pSceneObject.h"

class pScene
{
public:
	pScene();
	~pScene();

	pSceneGraph* sceneGraph;

	void initialize();

	/*Creates a sceneObject with a model, script and soundsystem.
	*Creates a sceneNode with a position, rotation, scale, name, and parent node. attaches the new sceneObject to the node
	*attaches the new node to the sceneGraph.
	*/
	void createNewObject(pModel* model, pScript* script, pSoundSystem* soundSystem, glm::vec3 & pos, glm::vec3 & rot, GLfloat amount, glm::vec3 & scaling, std::string objName, pSceneNode* parent);

private:
	


};

