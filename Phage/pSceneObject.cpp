#include "pSceneObject.h"

pSceneObject::pSceneObject()
{
	attachedModel = nullptr;
	attachedScript = nullptr;
	attachedSoundSystem = nullptr;
	attachedCamera = nullptr;
}

pSceneObject::pSceneObject(pModel * model, pScript * script, pSoundSystem* soundSystem, pCamera* camera)
{
	attachModel(model);
	attachScript(script);
	attachSoundSystem(soundSystem);
	attachCamera(camera);
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

void pSceneObject::attachCamera(pCamera * camera)
{
	detachCamera();
	attachedCamera = camera;
}

pCamera * pSceneObject::getAttachedCamera()
{
	return attachedCamera;
}

void pSceneObject::detachScript()
{
	attachedScript = nullptr;
}

void pSceneObject::attachAnimator(pAnimator * animator)
{
	detachAnimator();
	attachedAnimator = animator;
}

pAnimator * pSceneObject::getAttachedAnimator()
{
	return attachedAnimator;
}

void pSceneObject::detachAnimator()
{
	attachedAnimator = nullptr;
}

void pSceneObject::detachSoundSystem()
{
	attachedSoundSystem = nullptr;
}

void pSceneObject::detachCamera()
{
	attachedCamera = nullptr;
}

bool pSceneObject::hasScript() const
{
	return attachedScript != nullptr;
}

bool pSceneObject::hasSoundSystem() const
{
	return attachedSoundSystem != nullptr;
}

bool pSceneObject::hasCamera() const
{
	return (attachedCamera != nullptr);
}

bool pSceneObject::hasAnimator() const
{
	return (attachedAnimator != nullptr);
}

pSceneNode * pSceneObject::getSceneNode() const
{
	return node;
}

void pSceneObject::setSceneNode(pSceneNode* nde)
{
	node = nde;
}
