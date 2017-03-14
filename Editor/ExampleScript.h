#pragma once
#include "pScript.h"

class ExampleScript :
	public pScript
{
public:
	ExampleScript(pSceneObject* attatchedObject);
	~ExampleScript();

	// Inherited via pScript
	virtual void OnCreate() override;
	virtual void OnStart() override;
	virtual void OnUpdate(double deltaTime) override;
	virtual void OnDestroy() override;
	virtual void OnEnd() override;

private:
	GLfloat hover;
	const GLfloat yOffset = 2.0f;
};

