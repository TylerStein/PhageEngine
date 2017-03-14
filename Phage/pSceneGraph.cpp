#include "pSceneGraph.h"
#include "pScript.h"

void pSceneGraph::initialize()
{
	rootSceneNode = new pSceneNode();
}

void pSceneGraph::renderSceneGraph(pRenderer * renderer, pSceneNode* node)
{
	for (int i = 0; i < node->attachedSceneNodeList.size(); ++i)
	{
		matrixStack.push_back(node->getModelMatrix());
		renderSceneGraph(renderer, node->attachedSceneNodeList[i]);
	}
	if (node != rootSceneNode)
	{
		renderSceneNode(node, renderer);
	}
	if (!matrixStack.empty())
	{
		matrixStack.pop_back();
	}
}

void pSceneGraph::updateSceneGraph(double deltaTime, pSceneNode * node)
{
	for (int i = 0; i < node->attachedSceneNodeList.size(); ++i)
	{
		updateSceneGraph(deltaTime, node->attachedSceneNodeList[i]);
	}
	if (node != rootSceneNode)
	{
		updateSceneNode(node, deltaTime);
	}
}

pSceneNode * pSceneGraph::findSceneNode(std::string objName, pSceneNode* node)
{
	if (node == nullptr)
	{
		node = rootSceneNode;
	}

	if (node->getName() == objName) { return node; }

	pSceneNode* res = nullptr;

	for (int i = 0; i < node->attachedSceneNodeList.size(); ++i)
	{
		res = findSceneNode(objName, node->attachedSceneNodeList[i]);
		if (res)
		{
			if (res->getName() == objName) { return res; }
		}
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
	node->setScale(scaling);
	for (int i = 0; i < node->attachedSceneNodeList.size(); ++i)
	{
		applyTransform(node->attachedSceneNodeList[i], pos, rot, scaling);
	}
}


void pSceneGraph::renderSceneNode(pSceneNode * node, pRenderer * renderer)
{
	if (node != nullptr)
	{
		glm::mat4x4 tmp = node->getModelMatrix();

		//for (int i = 0; i < matrixStack.size(); ++i)
		for (int i = matrixStack.size() - 1; i >= 0; --i)
		{
			tmp = matrixStack[i] * tmp;
		}

		renderer->renderSceneNode(node, tmp);
	}

}

void pSceneGraph::updateSceneNode(pSceneNode * node, double deltaTime)
{
	if (node != nullptr) {
		if (node->getAttachedSceneObject()->hasScript()) {
			node->getAttachedSceneObject()->getAttachedScript()->OnUpdate(deltaTime);
		}
	}
}
