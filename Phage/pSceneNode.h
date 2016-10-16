#pragma once

#include <vector>
#include "glm/gtc/quaternion.hpp"
#include "pSceneObject.h"

class pSceneNode
{
public:

	//iterator to iterate through list of models in scene node
	typedef std::vector<pSceneObject*>::iterator sceneObject_list_iterator;

	//default constructor
	pSceneNode();

	//default  constructor
	~pSceneNode();

	/* constructor will be resposible for setting position to origin, rotataion up the Y axis, and set
	*scale to 1.0 on x, y, and z.
	*/
	pSceneNode(const glm::vec3 &pos, const glm::quat &rot, const glm::vec3 &scale);

	//method to set position
	void setPosition(const glm::vec3 &position);

	//returns number of sceneObjects attached to the scene node
	int numAttachedSceneOnjects() const;

	//returns current position of scene node, relative to parent
	glm::vec3 getLocation() const;

	//sets the rotation of the node, relative to its parent
	void setRotation(const glm::quat &rot);

	/*rotates a scene node about a point in the scene
	*point is the point it will rotate about
	*rotation is the specified rotation
	*/
	void setRotateAbout(const glm::vec3 &point, const glm::quat &rot);

	//returns the point, as well as the rotation used to rotate object around a point in the scene
	void getRotateAbout(glm::vec3 &point, glm::quat &rot);

	//returns the current rotation of the scene node
	glm::quat getRotation() const;

	//sets the scale factor of the scene node
	void setScale(const glm::vec3 &scaling);

	//returns the scene nodes current scale
	glm::vec3 getScale() const;

	//adds a child to the end of this nodes list of children
	void appendChild(pSceneNode * node);

	/*removes child indicated from the list of children on this node
	*does not delete the SceneObject
	*/
	void removeChild(pSceneNode *node);

	//returns pointer to the first child attached to this node. NULL if there is none.
	pSceneNode* getFirstChild() const;

	//returns pointer to next sibling attached to this node, or NULL if there is none.
	pSceneNode* getNextSibling() const;

	//returns an STL iterator through the list of sceneObjects attached to this scene node
	//references the first sceneObject in the list
	sceneObject_list_iterator sceneObjectBegin();

	//returns STL iterator through the list of sceneObjects attached to this scene node
	//references the last sceneObejct in the list
	sceneObject_list_iterator sceneObjectEnd();

	//allows users to store any required info within the scene node as type void*.
	//initialized to NULL, and will remain NULL until filled in by the user.
	void setUserData(void *uData)
	{
		userData = uData;
	}

	//returns user data
	void* getUserData()
	{
		return userData;
	}


private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 rotateAboutPoint;
	glm::quat rotateAboutOrientation;

	pSceneNode *firstChild;
	pSceneNode *lastChild;
	pSceneNode *previousSibling;
	pSceneNode *nextSibling;
	pSceneNode *parentNode;
	std::vector<pSceneObject*> sceneObjectList;
	void* userData;
};

