#pragma once

#include <vector>
#include "pSceneObject.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm\mat4x4.hpp"

class pSceneObject;

class pSceneNode
{
public:

	//List of attached nodes to this scene Node
	std::vector<pSceneNode*> attachedSceneNodeList;
	//iterator to iterate through list of models in scene node
	typedef std::vector<pSceneNode*>::iterator sceneNode_list_iterator;

	//default constructor
	pSceneNode();

	//default  constructor
	~pSceneNode();

	//set position, rotation, scale, name, attached sceneObject, and parent node. must have attached sceneObject
	pSceneNode(glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale, std::string &name, pSceneObject* attachedObject, pSceneNode* parentNode);

	//Translates relative to current position by <xyz>
	void translate(glm::vec3 pos);

	//method to set position
	void setPosition(glm::vec3 position);

	//returns number of sceneObjects attached to the scene node
	int numAttachedSceneNode() const;

	//returns current position of scene node, relative to parent
	glm::vec3 getPosition();

	//Adds rotation relative to current around a provided axis by <amount> degrees
	void rotateAround(glm::vec3 rot, GLfloat amount);

	//updates the up, forward, and right vectors
	void updateDirectionVectors();

	//Rotate via quaternion multiplication
	void rotate(glm::quat rotation);

	//rotates a scene node about a point in the scene. point is the point it will rotate about. rotation is the specified rotation **NOT IMPLIMENTED**
	//void setRotateAbout(const glm::vec3 &point, const glm::quat &rot);

	void setRotation(glm::vec3 rot);

	void setRotation(glm::quat rot);

	//returns the current rotation of the scene node
	glm::quat getRotationQuat();

	glm::vec3 getRotationEuler();

	//sets the scale factor of the scene node
	void setScale(const glm::vec3 scaling);

	//Scales relative to current scale by <xyz>
	void scaling(glm::vec3 scl);

	//returns the scene nodes current scale
	glm::vec3 getScale() const;

	void setForwardVector();

	inline glm::vec3 getForwardVector()
	{
		return forwardVector;
	}

	void setRightVector();

	inline glm::vec3 getRightVector()
	{
		return rightVector;
	}

	void setUpVector();

	inline glm::vec3 getUpVector()
	{
		return upVector;
	}

	//calculates the quaternion needed to rotate v1 so that it matches v2
	glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 destination);

	//returns a quaternion that will makew object look towards a given direction
	//direction is not target point, but the direction it wil look into
	glm::quat getLookAtQuat(glm::vec3 direction, glm::vec3 desiredUp = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4x4 getModelMatrix();

	void setTranslationMatrix();

	inline glm::mat4x4 getTranslationMatrix()
	{
		return translationMatrix;
	}

	void setRotationMatrix();

	inline glm::mat4x4 getRotationMatrix()
	{
		return rotationMatrix;
	}

	void setScaleMatrix();

	inline glm::mat4x4 getScaleMatrix()
	{
		return scaleMatrix;
	}

	inline glm::quat getLookAtPoint(glm::vec3 position, glm::vec3 desiredUp = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		glm::vec3 direction = glm::normalize(position - getPosition());
		return getLookAtQuat(direction, desiredUp);
	}

	inline void lookAtPoint(glm::vec3 positon, glm::vec3 desiredUp = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		glm::vec3 direction = glm::normalize(position - getPosition());
		rotation = getLookAtPoint(position, desiredUp);
	}

	glm::quat quaternionSlerp(glm::quat start, glm::quat end, float mixFactor);


	//sets name of sceneNode
	void setName(std::string objName);

	//returns name of sceneNode
	std::string getName() const;

	//adds a child to the end of this nodes list of children
	void appendChild(pSceneNode * node);

	/*removes child indicated from the list of children on this node
	*does not delete the SceneObject
	*/
	void removeChild(pSceneNode *node);

	//returns an STL iterator through the list of sceneObjects attached to this scene node
	//references the first sceneObject in the list
	sceneNode_list_iterator sceneNodeBegin();

	//returns STL iterator through the list of sceneObjects attached to this scene node
	//references the last sceneObejct in the list
	sceneNode_list_iterator sceneNodeEnd();

	pSceneObject* getAttachedSceneObject();

	void setAttachedSceneObject(pSceneObject* sceneObject)
	{
		attachedSceneObject = sceneObject;
	}

	void addSceneNode(pSceneNode* sceneObject);

	void removeSceneNode(pSceneNode* sceneObject);

	int getNumberSceneObjects() const;

	void initDefaultMatrix();


private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	glm::vec3 forwardVector;
	glm::vec3 rightVector;
	glm::vec3 upVector;

	glm::mat4x4 translationMatrix;
	glm::mat4x4 rotationMatrix;
	glm::mat4x4 scaleMatrix;

	glm::mat4x4 modelMatrix;

	std::string name;

	pSceneObject* attachedSceneObject;

	pSceneNode *parentNode;
};

