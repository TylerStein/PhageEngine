#include "pSceneObject.h"

pSceneObject::pSceneObject()
{
	attachedModel = nullptr;
	attachedScript = nullptr;
	attachedSoundSystem = nullptr;
}

pSceneObject::pSceneObject(pModel * model, pScript * script, pSoundSystem* soundSystem)
{
	attachModel(model);
	attachScript(script);
	attachSoundSystem(soundSystem);
}

void pSceneObject::attachModel(pModel * model)
{
	attachedModel = model;
}

void pSceneObject::detachModel()
{
	attachedModel = nullptr;
}

bool pSceneObject::hasModel() const
{
	return attachedModel != nullptr;
}

pModel * pSceneObject::getAttachedModel() const
{
	return attachedModel;
}

void pSceneObject::attachScript(pScript * script)
{
	detachScript();
	attachedScript = script;
}

void pSceneObject::attachSoundSystem(pSoundSystem * soundSystem)
{
	detachSoundSystem();
	attachedSoundSystem = soundSystem;
}

void pSceneObject::detachScript()
{
	attachedScript = nullptr;
}

void pSceneObject::detachSoundSystem()
{
	attachedSoundSystem = nullptr;
}

bool pSceneObject::hasScript() const
{
	return attachedScript != nullptr;
}

bool pSceneObject::hasSoundSystem() const
{
	return attachedSoundSystem != nullptr;
}
