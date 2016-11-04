#pragma once

#include <string>
#include "pMaterial.h"
#include "pModel.h"
#include <map>
#include "pScript.h"
#include "pRenderer.h"
#include "pSceneNode.h"

class pSceneObject
{

public:
	//Create an empty game object with a given name
	pSceneObject(std::string& name);

	/**
	* This creates a game object and attaches it to the root of the scene graph. Either or both of the
	* model and script can be null and can be set later, or left as null if they are not required
	*/
	pSceneObject(std::string& name, pModel* model, pScript* script);

	/**
	* This creates a game object and attaches it as a child of the indicated parent game object. Either or both of the
	* model and script can be null and can be set later, or left as null if they are not required
	*/
	pSceneObject(std::string& name, pSceneObject* parent, pModel* model, pScript* script);

	std::string getName() const;

	void attachToSceneNode(pSceneNode* node);

	void detachFromSceneNode();

	//Determine if this object is attached to the scene graph or not
	bool isAttached() const;

	//This attaches a model to the game object. A game object without a model will render nothing
	void attachModel(pModel* model);

	//This will detach the model from the game object. If no model is attached, no action is taken.	
	void detachModel();

	bool hasModel() const;

	//Return a pointer to the attached model or nullptr if no model is attached.
	pModel* getAttachedModel() const;

	// Attach a script object to this game object
	void attachScript(pScript* script);

	void detachScript();

	bool hasScript() const;

	void attachChild(pSceneObject* child);

	void detachChild(pSceneObject* child);

private:
		std::string objectName;
		pSceneNode* sceneNode;
		pModel* attachedModel;
		std::string materialName;
		pScript* attachedScript = nullptr;

};