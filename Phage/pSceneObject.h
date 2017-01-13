#pragma once

#include <string>
#include "pMaterial.h"
#include "pModel.h"
#include <map>
#include "pScript.h"
#include "pRenderer.h"
#include "pSceneNode.h"
#include "pSoundSystem.h"

class pSceneObject
{

public:
	//Create an empty game object with a given name
	pSceneObject(std::string name);

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

	// Attach a script object to this game object. if one is already attached.
	//if one is already attached it will be removed and replaced with the new one
	void attachScript(pScript* script);

	//attaches a sound System to this game object
	//if one is already attached it will be removed and replaced with the new one
	void attachSoundSystem(pSoundSystem* soundSystem);

	void detachScript();

	//detaches a script from this game object
	void detachSoundSystem();

	bool hasScript() const;

	bool hasSoundSystem() const;

	void attachChild(pSceneObject* child);

	void detachChild(pSceneObject* child);

private:
		std::string objectName;
		pSceneNode* sceneNode;
		pModel* attachedModel;
		std::string materialName;
		pScript* attachedScript = nullptr;
		pSoundSystem* attachedSoundSystem = nullptr;
};