#include "pSceneObject.h"

pSceneObject::pSceneObject(std::string  name)
{
	objectName = name;
	sceneNode = nullptr;
	attachedModel = nullptr;
}

pSceneObject::pSceneObject(std::string & name, pModel * model, pScript * script)
{
	objectName = name;
	sceneNode = new pSceneNode();
	sceneNode->addSceneObject(this);
	attachModel(model);
	attachScript(script);
}

pSceneObject::pSceneObject(std::string & name, pSceneObject * parent, pModel * model, pScript * script)
{
	objectName = name;
	parent->attachChild(this);
	attachModel(model);
	attachScript(script);
}

std::string pSceneObject::getName() const
{
	return objectName;
}

void pSceneObject::attachToSceneNode(pSceneNode * node)
{
	if (isAttached())
	{
		detachFromSceneNode();
	}
	sceneNode = node;
	sceneNode->addSceneObject(this);
}

void pSceneObject::detachFromSceneNode()
{
	if (isAttached())
	{
		sceneNode->removeSceneObject(this);
		sceneNode = nullptr;
	}
}

bool pSceneObject::isAttached() const
{
	return sceneNode != nullptr;
}

void pSceneObject::attachModel(pModel * model)
{
	attachedModel = model;
	std::string modelMat = model->getName();
	if (modelMat.length() > 0)
	{
		materialName = modelMat;
	}
	else
	{
		if (materialName.length() > 0)
		{
			//TODO::define set material name method for pMaterial class
			//model->setMaterialName(materialName);
		}
	}
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

void pSceneObject::attachChild(pSceneObject * child)
{
	pSceneNode* tmp = new pSceneNode();
	sceneNode->appendChild(tmp);
	tmp->addSceneObject(child);
}

void pSceneObject::detachChild(pSceneObject * child)
{
	pSceneNode* childPtr = sceneNode->getFirstChild();
	while (childPtr != nullptr)
	{
		pSceneNode::sceneObject_list_iterator iter;
		for (iter = childPtr->sceneObjectBegin(); iter != childPtr->sceneObjectEnd(); iter++)
		{
			int location = std::distance(childPtr->sceneObjectBegin(), iter);
			if (childPtr->sceneObjectList[location] == child)
			{
				childPtr->removeSceneObject(child);
				if (childPtr->getNumberSceneObjects() == 0)
				{
					sceneNode->removeChild(childPtr);
				}
				return;
			}
		}

		childPtr = childPtr->getNextSibling();
	}
}
