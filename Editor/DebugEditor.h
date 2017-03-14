#pragma once
#include "PhageEngine.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#include "pCamera.h"

class DebugEditor : public GameImplement
{
public:
	DebugEditor();
	~DebugEditor();

	double lastTime;
	int numFrames;

	float lastFrameTime;

	double lastMouseX, lastMouseY;
	glm::mat4 viewMat;

	void giveEngineReference(PhageEngine* engine);

	pSceneObject* cameraObject;

	// Inherited via GameImplement
	virtual void onStart() override;
	virtual void onUpdate(GLdouble deltaTime) override;
	virtual void onPostUpdate() override;
	virtual void onPreRender() override;
	virtual void onRender() override;
	virtual void onPostRender() override;
	virtual void onEnd() override;

private:
	PhageEngine* engine;
};

