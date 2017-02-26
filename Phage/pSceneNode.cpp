#include "pSceneNode.h"
#include <algorithm>
#include "LogManager.h"

#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

pSceneNode::pSceneNode()
{
	setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	setRotation(glm::vec3(0.0f, 0.0f, 0.0f), 0);
	setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	name = "rootNode";
	attachedSceneObject = new pSceneObject();
	parentNode = nullptr;
	initDefaultMatrix();
}


pSceneNode::~pSceneNode()
{
}

pSceneNode::pSceneNode(glm::vec3 & pos, glm::vec3 & rot, GLfloat amount, glm::vec3 & scaling, std::string & objName, pSceneObject* attachedObject, pSceneNode* parent)
{
	setPosition(pos);
	setRotation(rot, amount);
	setScale(scaling);
	name = objName;
	attachedSceneObject = attachedObject;
	parentNode = parent;
	parentNode->appendChild(this);

	getModelMatrix();
}

int pSceneNode::numAttachedSceneNode() const
{
	return attachedSceneNodeList.size();
}

void pSceneNode::translate(glm::vec3 pos)
{
	translationMatrix = glm::translate(translationMatrix, pos);
	//getAttachedSceneObject()->getAttachedModel()->getTranslationMatrix() = glm::translate(getAttachedSceneObject()->getAttachedModel()->getTranslationMatrix(), pos);
}

void pSceneNode::setPosition(glm::vec3 pos)
{
	translationMatrix = glm::translate(glm::mat4(1.0f), pos);
}

/*glm::vec3 pSceneNode::getLocation()
{
	glm::vec3 result = position;
	return result;
}*/

void pSceneNode::rotateAround(glm::vec3 rot, GLfloat amount)
{
	rotationMatrix *= glm::rotate(glm::radians(amount), rot);
}

void pSceneNode::setRotation(glm::vec3 rot, GLfloat amount)
{
	rotationMatrix = glm::rotate(glm::radians(amount), rot);
}

/*void pSceneNode::setRotateAbout(const glm::vec3 & point, const glm::quat & rot)
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
}*/

void pSceneNode::scale(glm::vec3 scl)
{
	scaleMatrix = glm::scale(scaleMatrix, scl);
	//call same method on model
}

void pSceneNode::setScale(const glm::vec3 scaling)
{
	scaleMatrix = glm::scale(glm::mat4(1.0f), scaling);
}

/*glm::vec3 pSceneNode::getScale() const
{
	glm::vec3 result = scale;
	return result;
}*/

glm::mat4 pSceneNode::getModelMatrix()
{
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	return modelMatrix;
}

void pSceneNode::setName(std::string objName)
{
	name = objName;
}

std::string pSceneNode::getName() const
{
	return name;
}

void pSceneNode::appendChild(pSceneNode * node)
{
	attachedSceneNodeList.push_back(node);
}

void pSceneNode::removeChild(pSceneNode * node)
{
	pSceneNode *current;

}

pSceneNode::sceneNode_list_iterator pSceneNode::sceneNodeBegin()
{
	return attachedSceneNodeList.begin();
}

pSceneNode::sceneNode_list_iterator pSceneNode::sceneNodeEnd()
{
	return attachedSceneNodeList.end();
}

pSceneObject * pSceneNode::getAttachedSceneObject()
{
	return attachedSceneObject;
}

glm::vec3 pSceneNode::getForwardVector()
{
	return glm::vec3();
}

glm::vec3 pSceneNode::getRightVector()
{
	return glm::vec3();
}

void pSceneNode::addSceneNode(pSceneNode * sceneObject)
{
	attachedSceneNodeList.push_back(sceneObject);
}

void pSceneNode::removeSceneNode(pSceneNode * sceneObject)
{
	sceneNode_list_iterator iter = std::find(attachedSceneNodeList.begin(), attachedSceneNodeList.end(), sceneObject);
	if (iter != attachedSceneNodeList.end())
	{
		attachedSceneNodeList.erase(iter);
	}
}

int pSceneNode::getNumberSceneObjects() const
{
	return attachedSceneNodeList.size();
}

void pSceneNode::initDefaultMatrix()
{
	rotationMatrix = glm::mat4(1.0f);
	translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
}
