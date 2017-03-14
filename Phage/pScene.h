#pragma once

#include "pSceneGraph.h"

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
	pSceneObject* createNewObject(pModel* model = nullptr, pScript* script = nullptr, pSoundSystem* soundSystem = nullptr, pCamera* camera = nullptr, 
		glm::vec3 & pos = glm::vec3(0), glm::vec3 & rot = glm::vec3(0), glm::vec3 & scaling = glm::vec3(1), std::string objName = "UNNAMED_OBJECT", pSceneNode* parent = nullptr);

	//Adds the tree of nodes (starting at treeRoot) to the scene under provided parent (or root)
	pSceneNode* addExistingNodes(pSceneNode* treeRoot, pSceneNode* parent = nullptr);

};

