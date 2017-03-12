#pragma once

#include <vector>
#include "pSceneObject.h"
#include "glm/gtc/quaternion.hpp"
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

	/*main Constructor. will set position, rotation and scale in world, name of node, the attached object, and specify parent node.
	*set parent node to null if you want parent node to be root node
	*must have attached sceneObject
	*/
	pSceneNode(glm::vec3 &pos, glm::vec3 &rot, GLfloat amount, glm::vec3 &scale, std::string &name, pSceneObject* attachedObject, pSceneNode* parentNode);

	//Translates relative to current position by <xyz>
	void translate(glm::vec3 pos);

	//method to set position
	void setPosition(glm::vec3 position);

	//returns number of sceneObjects attached to the scene node
	int numAttachedSceneNode() const;

	//returns current position of scene node, relative to parent
	//glm::vec3 getLocation();

	//Adds rotation relative to current around a provided axis by <amount> degrees
	void rotateAround(glm::vec3 rot, GLfloat amount);

	//sets the rotation of the node, relative to its parent
	void setRotation(glm::vec3 rot, GLfloat amount);

	/*rotates a scene node about a point in the scene
	*point is the point it will rotate about
	*rotation is the specified rotation
	*/
	//void setRotateAbout(const glm::vec3 &point, const glm::quat &rot);

	//returns the point, as well as the rotation used to rotate object around a point in the scene
	//void getRotateAbout(glm::vec3 &point, glm::quat &rot);

	//returns the current rotation of the scene node
	//glm::quat getRotation() const;

	//sets the scale factor of the scene node
	void setScale(const glm::vec3 scaling);

	//Scales relative to current scale by <xyz>
	void scale(glm::vec3 scl);

	//returns the scene nodes current scale
	//glm::vec3 getScale() const;

	glm::mat4 getModelMatrix();

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

	glm::vec3 getForwardVector();

	glm::vec3 getRightVector();

	void addSceneNode(pSceneNode* sceneObject);

	void removeSceneNode(pSceneNode* sceneObject);

	int getNumberSceneObjects() const;

	void initDefaultMatrix();


private:
	//glm::vec3 position;
	//glm::quat rotation;
	//glm::vec3 scale;
	//glm::vec3 rotateAboutPoint;
	//glm::quat rotateAboutOrientation;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	glm::mat4 modelMatrix;

	std::string name;

	pSceneObject* attachedSceneObject;

	pSceneNode *parentNode;
};

