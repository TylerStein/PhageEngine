#include "pSceneGraph.h"

void pSceneGraph::initialize()
{
	rootSceneNode = new pSceneNode();
}

void pSceneGraph::renderSceneGraph(pFrustum &frustum, pRenderer &renderer, pCamera *camera)
{
	renderOffset = glm::vec3(0.0f, 0.0f, 0.0f);

	//TO DO::call renderers render function to render to the camera

	renderSceneNode(rootSceneNode, frustum, renderer, camera);
}


void pSceneGraph::renderSceneNode(pSceneNode * sceneRoot, pFrustum & frustum, pRenderer & renderer, pCamera * camera)
{
	//TO DO::call renderers renderSceneNode, and pass  reference to the scene node to be rendererd, as well as the camera

	renderOffset = renderOffset + sceneRoot->getLocation();
	pSceneNode::sceneObject_list_iterator miter = sceneRoot->sceneObjectBegin();
	while (miter != sceneRoot->sceneObjectEnd())
	{

		//TO DO:: prepare SceneObject to be rendered

		miter++;
	}

	pSceneNode *child = sceneRoot->getFirstChild();
	while (child != NULL)
	{
		renderSceneNode(child, frustum, renderer, camera);
		child = child->getNextSibling();
	}

	renderOffset = renderOffset - sceneRoot->getLocation();

	//TO DO::call renderers renderSceneNode, and pass  reference to the scene node to be rendererd, as well as the camera
}
