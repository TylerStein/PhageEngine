#include "pSceneGraph.h"

void pSceneGraph::initialize()
{
	rootSceneNode = new pSceneNode();
}

void pSceneGraph::renderSceneGraph(pRenderer * renderer, pSceneNode* node)
{
	for (int i = 0; i < node->attachedSceneNodeList.size(); ++i)
	{
		renderSceneGraph(renderer, node->attachedSceneNodeList[i]);
	}
	if (node != rootSceneNode)
	{
		renderSceneNode(node, renderer);
	}
}

pSceneNode * pSceneGraph::findSceneNode(std::string objName, pSceneNode* node)
{
	if (node->getName() == objName) { return node; }

	pSceneNode* res = nullptr;

	for (int i = 0; i < node->attachedSceneNodeList.size(); ++i)
	{
		res = findSceneNode(objName, node->attachedSceneNodeList[i]);
	}

	return res;

}

void pSceneGraph::attachToRootNode(pSceneNode * node)
{
	rootSceneNode->appendChild(node);
}

void pSceneGraph::applyTransform(pSceneNode * node, glm::vec3 & pos, glm::quat & rot, glm::vec3 & scaling)
{
	node->setPosition(pos);
	//node->setRotation(rot);
	node->setScale(scaling);
	for (int i = 0; i < node->attachedSceneNodeList.size(); ++i)
	{
		applyTransform(node->attachedSceneNodeList[i], pos, rot, scaling);
	}
}


void pSceneGraph::renderSceneNode(pSceneNode * node, pRenderer * renderer)
{

	//pSceneNode* nodeToRender = node->getAttachedSceneObject();
	//pModel* modelToRender = sceneObject->getAttachedModel();
	if (node != nullptr)
	{
		renderer->renderSceneNode(node);
	}

}
