#include "pSceneNode.h"
#include <algorithm>
#include "LogManager.h"

#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include <math.h>

pSceneNode::pSceneNode()
{
	setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	name = "rootNode";
	attachedSceneObject = new pSceneObject();
	parentNode = nullptr;

	initDefaultMatrix();
	setForwardVector();
	setRightVector();
	setUpVector();
}


pSceneNode::~pSceneNode()
{
}

pSceneNode::pSceneNode(glm::vec3 & pos, glm::vec3 & rot, glm::vec3 & scaling, std::string & objName, pSceneObject* attachedObject, pSceneNode* parent)
{
	setPosition(pos);
	setRotation(rot);
	setScale(scaling);
	name = objName;
	attachedSceneObject = attachedObject;
	parentNode = parent;
	parentNode->appendChild(this);

	getModelMatrix();
	setForwardVector();
	setRightVector();
	setUpVector();
}

int pSceneNode::numAttachedSceneNode() const
{
	return attachedSceneNodeList.size();
}

void pSceneNode::translate(glm::vec3 pos)
{
	position += pos;
}

void pSceneNode::setPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 pSceneNode::getPosition()
{
	return position;
}

void pSceneNode::rotateAround(glm::vec3 rot, GLfloat amount)
{
	glm::quat tmp = glm::quat(rot * amount);
	rotation *= tmp;
}

void pSceneNode::setRotation(glm::vec3 rot)
{
	glm::quat tmp = glm::quat(rot);
	rotation = tmp;
}

void pSceneNode::setRotation(glm::quat rot)
{
	rotation = rot;
}

glm::quat pSceneNode::getRotationQuat()
{
	return rotation;
}

//un-normalized Quaternion
glm::vec3 pSceneNode::getRotationEuler()
{
	double sqw = rotation.w * rotation.w;
	double sqx = rotation.x * rotation.x;
	double sqy = rotation.y * rotation.y;
	double sqz = rotation.z * rotation.z;

	double heading;
	double attitude;
	double bank;

	glm::vec3 euler;

	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = rotation.x * rotation.y + rotation.z * rotation.w;
	if (test > 0.499 * unit) // singularity at north pole
	{
		heading = 2 * atan2(rotation.x, rotation.w);
		attitude = glm::pi<float>() / 2;
		bank = 0;

		euler = glm::vec3(bank, heading, attitude);
		return euler;
	}

	if (test < -0.499 * unit) // singularity at south pole
	{
		heading = -2 * atan2(rotation.x, rotation.w);
		attitude = -glm::pi<float>() / 2;
		bank = 0;

		euler = glm::vec3(bank, heading, attitude);
		return euler;
	}

	heading = atan2(2 * rotation.y * rotation.w - 2 * rotation.x * rotation.z, sqx - sqy - sqz + sqw);
	attitude = asin(2 * test / unit);
	bank = atan2(2 * rotation.x * rotation.w - 2 * rotation.y * rotation.z, -sqx + sqy - sqz + sqw);

	euler = glm::vec3(bank, heading, attitude);
	return euler;
} 

void pSceneNode::scaling(glm::vec3 scl)
{
	scale *= scl;
}

void pSceneNode::setScale(const glm::vec3 scaling)
{
	scale = scaling;
}

glm::vec3 pSceneNode::getScale() const
{
	return scale;
}

void pSceneNode::setForwardVector()
{
	glm::mat4x4 tmp = getModelMatrix();
	forwardVector = glm::vec3(tmp[2].x, tmp[2].y, tmp[2].z);
}

void pSceneNode::setRightVector()
{
	glm::mat4x4 tmp = getModelMatrix();
	rightVector = glm::vec3(tmp[0].x, tmp[0].y, tmp[0].z);
}

void pSceneNode::setUpVector()
{
	glm::mat4x4 tmp = getModelMatrix();
	upVector = glm::vec3(tmp[1].x, tmp[1].y, tmp[1].z);
}

glm::quat pSceneNode::rotationBetweenVectors(glm::vec3 start, glm::vec3 destination)
{
	start = glm::normalize(start);
	destination = glm::normalize(destination);

	float cosTheta = glm::dot(start, destination);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f)
	{
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01)
		{
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
		}

		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = glm::cross(start, destination);

	float s = glm::sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
}

glm::quat pSceneNode::getLookAtQuat(glm::vec3 direction, glm::vec3 desiredUp)
{
	if (glm::length2(direction) < 0.0001f)
	{
		return glm::quat();
	}

	//recalculate the desired up vector so it is perpendicular to the direction
	desiredUp = glm::cross(rightVector, direction);

	//find rotation between front of object and desired direction
	glm::quat rot1 = rotationBetweenVectors(forwardVector, direction);
	//fix up direction
	glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::quat rot2 = rotationBetweenVectors(newUp, desiredUp);

	//apply rotations
	return rot2 * rot1;
}

glm::mat4x4 pSceneNode::getModelMatrix()
{
	//get most updated matrices
	setTranslationMatrix();
	setRotationMatrix();
	setScaleMatrix();

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	return modelMatrix;
}

void pSceneNode::setTranslationMatrix()
{
	translationMatrix = glm::translate(glm::mat4x4(), position);
}

void pSceneNode::setRotationMatrix()
{
	rotationMatrix = glm::toMat4(rotation);
}

void pSceneNode::setScaleMatrix()
{
	scaleMatrix = glm::scale(glm::mat4x4(), scale);
}

glm::quat pSceneNode::quaternionSlerp(glm::quat start, glm::quat end, float mixFactor)
{
	return glm::mix(start, end, mixFactor);
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
	rotationMatrix = glm::mat4x4(1.0f);
	translationMatrix = glm::mat4x4(1.0f);
	scaleMatrix = glm::mat4x4(1.0f);
	modelMatrix = glm::mat4x4(1.0f);
}
