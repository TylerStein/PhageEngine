#pragma once
#include "PhageEngine.h"
#include "pLight.h"

class TestScene : public GameImplement
{
public:
	pShader* simpleShader;
	pImage* testImg;
	pMaterial* testMat;
	pModel* testModel;
<<<<<<< HEAD
	pModelLoader* modelLoader;
	pModel* loadedModel;
=======
	pSoundSystem *soundSystem;
>>>>>>> refs/remotes/origin/Development

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

