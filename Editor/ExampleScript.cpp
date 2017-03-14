#include "ExampleScript.h"
#include "pSceneNode.h"

ExampleScript::ExampleScript(pSceneObject * attatchedObject)
	: pScript("ExampleScript", attatchedObject)
{
}

ExampleScript::~ExampleScript()
{
}

void ExampleScript::OnCreate()
{
}

void ExampleScript::OnStart()
{
	hover = 0.0f;
}

void ExampleScript::OnUpdate(double deltaTime)
{
	hover += deltaTime;
	if (hover > 360) {
		hover = hover - 360;
	}
	
	sceneObject->getSceneNode()->rotateAround(glm::vec3(0, 1, 0), (float)deltaTime * 1.5f);
	sceneObject->getSceneNode()->setPosition(glm::vec3(0, (sin(hover / 10) * yOffset), 0) - (yOffset * 0.5f));
}

void ExampleScript::OnDestroy()
{
}

void ExampleScript::OnEnd()
{
}
