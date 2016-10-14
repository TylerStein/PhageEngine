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

pSceneNode::pSceneNode(const glm::vec3 & pos, const glm::quat & rot, const glm::vec3 & scaling)
{
	position = pos;
	rotation = rot;
	scale = scaling;
	firstChild = lastChild = previousSibling = nextSibling = parentNode = NULL;
	userData = NULL;
}

void pSceneNode::setPosition(const glm::vec3 & pos)
{
	position = pos;
}

int pSceneNode::numAttachedSceneOnjects() const
{
	return sceneObjectList.size();
}

glm::vec3 pSceneNode::getLocation() const
{
	glm::vec3 result = position;
	return result;
}

void pSceneNode::setRotation(const glm::quat &rot)
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
