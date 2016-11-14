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
	void renderSceneGraph (pSceneNode * sceneRoot, pRenderer * renderer);

private:
	pSceneNode *rootSceneNode;
	glm::vec3 renderOffset;
	pRenderer *rndr;

	void initialize();

	void renderSceneNode(pSceneNode *sceneRoot, pRenderer *renderer);
};

