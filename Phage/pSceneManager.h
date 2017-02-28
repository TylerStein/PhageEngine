#pragma once

#include "pScene.h"

class pSceneManager
{
public:
	static pSceneManager* instance();

	//update scripts attached to sceneObjects
	void updateScene(double deltaTime);

	//traverses scene graph and renders all sceneObjects
	void renderScene(pRenderer* renderer);

	//creates a new scene and adds it to the list
	void addScene();

	inline pScene* getCurrentScene()
	{
		return currentScene;
	}

private:

	pSceneManager();
	~pSceneManager();

	static pSceneManager* _instance;

	//keeps track of all scenes that exist within the game
	std::vector<pScene*> sceneList;

	//keeps track of the current scene that is being used
	pScene* currentScene;
};

