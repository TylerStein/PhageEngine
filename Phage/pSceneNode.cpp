#include "pSceneNode.h"
#include <algorithm>
#include "LogManager.h"

pSceneNode::pSceneNode()
{
	firstChild = lastChild = previousSibling = nextSibling = parentNode = NULL;
	rotation = glm::quat(0.0, 0.0, 1.0, 0.0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	userData = NULL;
}


pSceneNode::~pSceneNode()
{
	if (userData)
	{
		delete userData;
		userData = NULL;
	}
}

pSceneNode::pSceneNode(glm::vec3 & pos, glm::quat & rot, glm::vec3 & scaling)
{
	position = pos;
	rotation = rot;
	scale = scaling;
	firstChild = lastChild = previousSibling = nextSibling = parentNode = NULL;
	userData = NULL;
}

void pSceneNode::setPosition(glm::vec3 & pos)
{
	position = pos;
}

int pSceneNode::numAttachedSceneOnjects() const
{
	return sceneObjectList.size();
}

glm::vec3 pSceneNode::getLocation()
{
	glm::vec3 result = position;
	return result;
}

void pSceneNode::setRotation(glm::quat &rot)
{
	rotation = rot;
}

void pSceneNode::setRotateAbout(const glm::vec3 & point, const glm::quat & rot)
{
	rotateAboutPoint = point;
	rotateAboutOrientation = rot;
}

void pSceneNode::getRotateAbout(glm::vec3 & point, glm::quat & rot)
{
	point = rotateAboutPoint;
	rot = rotateAboutOrientation;
}

glm::quat pSceneNode::getRotation() const
{
	glm::quat result = rotation;
	return result;
}

void pSceneNode::setScale(const glm::vec3 &scaling)
{
	scale = scaling;
}

glm::vec3 pSceneNode::getScale() const
{
	glm::vec3 result = scale;
	return result;
}

void pSceneNode::appendChild(pSceneNode * node)
{
	if(lastChild == NULL)
	{
		firstChild = node;
	}
	else
	{
		lastChild->nextSibling = node;
		node->previousSibling = lastChild;
	}
	lastChild = node;
	node->parentNode = this;
}

void pSceneNode::removeChild(pSceneNode * node)
{
	pSceneNode *current;

	for (current = firstChild; current != NULL; current = current->nextSibling)
	{
		if (current == node)
		{
			if (node->previousSibling)
			{
				node->previousSibling->nextSibling = node->nextSibling;
			}
			else
			{
				firstChild = node->nextSibling;
			}
			if (node->nextSibling)
			{
				node->nextSibling->previousSibling = node->previousSibling;
			}
			else
			{
				lastChild = node->previousSibling;
			}
			node->parentNode = NULL;
			return;
		}
	}
}

pSceneNode * pSceneNode::getFirstChild() const
{
	return firstChild;
}

pSceneNode * pSceneNode::getNextSibling() const
{
	return nextSibling;
}

pSceneNode::sceneObject_list_iterator pSceneNode::sceneObjectBegin()
{
	return sceneObjectList.begin();
}

pSceneNode::sceneObject_list_iterator pSceneNode::sceneObjectEnd()
{
	return sceneObjectList.end();
}

void pSceneNode::addSceneObject(pSceneObject * sceneObject)
{
	sceneObjectList.push_back(sceneObject);
}

void pSceneNode::removeSceneObject(pSceneObject * sceneObject)
{
	sceneObject_list_iterator iter = std::find(sceneObjectList.begin(), sceneObjectList.end(), sceneObject);
	if (iter != sceneObjectList.end())
	{
		sceneObjectList.erase(iter);
	}
}

int pSceneNode::getNumberSceneObjects() const
{
	return sceneObjectList.size();
}
