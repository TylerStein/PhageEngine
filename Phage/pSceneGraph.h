#pragma once

#include "pSceneNode.h"
#include "pRenderer.h"
#include "pSceneObject.h"

class pSceneGraph
{
public:

	//creates empty scene graph
	pSceneGraph()
	{
		initialize();
	}

	//returns pointer to root scene node in the scene graph
	pSceneNode* getRootSceneNode()
	{
		return rootSceneNode;
	}

	//render the scene graph using the renderer. 
	void renderSceneGraph (pRenderer * renderer, pSceneNode* node);

	//find a specific object by name and return a reference to it
	pSceneNode* findSceneNode(std::string objName, pSceneNode* node = nullptr);

	void attachToRootNode(pSceneNode* node);

	//apply transformations to all objects in scene
	void applyTransform(pSceneNode * node, glm::vec3 & pos, glm::quat & rot, glm::vec3 & scaling);

private:
	pSceneNode* rootSceneNode;
	pRenderer* rndr;

	std::vector<glm::mat4x4> matrixStack;

	void initialize();

	void renderSceneNode(pSceneNode *sceneRoot, pRenderer *renderer);
};

