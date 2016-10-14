#pragma once

#include "pSceneNode.h"

class pFrustum;
class pRenderer;
class pCamera;

class pSceneGraph
{
public:

	//creates empty scene graph
	pSceneGraph(float worldSz, std::string indexType) : worldSize(worldSz)
	{
		initialize();
	}

	//returns pointer to root scene node in the scene graph
	pSceneNode* getRootSceneNode()
	{
		return rootSceneNode;
	}

	//returns world size
	int getWorldSize()
	{
		return worldSize;
	}

	//render the scene graph using the renderer. 
	void renderSceneGraph (pFrustum &frustum, pRenderer &renderer, pCamera *camera);

private:
	pSceneNode *rootSceneNode;
	float worldSize;
	glm::vec3 renderOffset;

	void initialize();

	void renderSceneNode(pSceneNode *sceneRoot, pFrustum &frustum, pRenderer &renderer, pCamera *camera);
};

