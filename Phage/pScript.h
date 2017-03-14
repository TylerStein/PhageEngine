#pragma once
#include "pSceneObject.h"
#include <string>

//Forward declare pSceneObject to avoid circular include with SceneObject, as it requires pScript
//class pSceneObject;

class pScript : public pAsset
{
	friend class pScriptManager;

public:

	inline pScript(std::string name, pSceneObject* parentObject) {
		this->name = name;
		sceneObject = parentObject;
	}

	inline ~pScript() {}

	virtual void OnCreate() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate(double deltaTime) = 0;
	virtual void OnDestroy() = 0;
	virtual void OnEnd() = 0;
protected:
	pSceneObject* sceneObject;
};

