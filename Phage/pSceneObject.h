#pragma once

#include <string>
#include <map>

#include "pModel.h"
#include "pSoundSystem.h"
#include "pCamera.h"


class pScript;
class pSceneNode;


class pSceneObject
{
	friend class pScene;
public:
	//Create an empty game object
	pSceneObject();

	//Constructor for sceneObject where you can supply a model, script, or sound system. set to null if you do not want to attach a component
	pSceneObject(pModel* model, pScript* script, pSoundSystem* soundSystem, pCamera* camera);

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

	inline pScript* getAttachedScript()
	{
		return attachedScript;
	}

	//attaches a sound System to this game object
	//if one is already attached it will be removed and replaced with the new one
	void attachSoundSystem(pSoundSystem* soundSystem);

	inline pSoundSystem* getAttachedSoundSystem()
	{
		return attachedSoundSystem;
	}

	//attaches a camera to the sceneObject
	void attachCamera(pCamera* camera);

	inline pCamera* getAttachedCamera()
	{
		return attachedCamera;
	}

	void detachScript();

	//detaches a script from this game object
	void detachSoundSystem();

	void detachCamera();

	bool hasScript() const;

	bool hasSoundSystem() const;

	bool hasCamera() const;

	pSceneNode* getSceneNode() const;

private:
	pModel* attachedModel = nullptr;
	pScript* attachedScript = nullptr;
	pSoundSystem* attachedSoundSystem = nullptr;
	pCamera* attachedCamera = nullptr;

	pSceneNode* node;
};