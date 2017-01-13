#pragma once
#include "PhageEngine.h"
#include "pLight.h"

class TestScene : public GameImplement
{
public:
	pModel* crateRef;
	pModel* chairRef;
	pModel* barrelRef;
	pModel* floorRef;

	pModel* mdl_LightA;
	pModel* mdl_LightB;
	pModel* mdl_LightC;

	pLight* light_A;
	pLight* light_B;
	pLight* light_C;

	double lastMouseX, lastMouseY;

	TestScene();
	~TestScene();

	void giveEngineReference(PhageEngine* engine);

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
	bool isLinkedToEngine;
};

