#pragma once
#include "PhageEngine.h"

class TestScene : public GameImplement
{
public:
	pImage* testImg;
	pMaterial* testMat;
	pModel* testModel;

	TestScene();
	~TestScene();

	void giveEngineReference(PhageEngine* engine);

	// Inherited via GameImplement
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onPostUpdate() override;
	virtual void onPreRender() override;
	virtual void onRender() override;
	virtual void onPostRender() override;
	virtual void onEnd() override;
private:
	PhageEngine* engine;
	bool isLinkedToEngine;
};

