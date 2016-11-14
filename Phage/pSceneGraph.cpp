#include "pSceneGraph.h"

void pSceneGraph::initialize()
{
	rootSceneNode = new pSceneNode();
}

void pSceneGraph::renderSceneGraph(pSceneNode * sceneRoot, pRenderer * renderer)
{
	renderSceneNode(rootSceneNode, renderer);
	pSceneNode* nodeToRender = rootSceneNode->getFirstChild();
	while (nodeToRender != nullptr)
	{
		renderSceneNode(nodeToRender, renderer);
		nodeToRender = nodeToRender->getNextSibling();
	}
}


void pSceneGraph::renderSceneNode(pSceneNode * node, pRenderer * renderer)
{
	for (int i = 0; i < node->sceneObjectList.size(); i++)
	{
		pSceneObject* sceneObject = node->sceneObjectList[i];
		pModel* modelToRender = sceneObject->getAttachedModel();
		if (modelToRender != nullptr)
		{
			renderer->renderModel(modelToRender);
		}
	}
}
