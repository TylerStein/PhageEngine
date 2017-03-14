#pragma once

#include "pScene.h"

class pSceneManager
{
public:
	static pSceneManager* instance();

	//start scripts attached to sceneObjects
	void startScene();

	//update scripts attached to sceneObjects
	void updateScene(double deltaTime);

	//traverses scene graph and renders all sceneObjects
	void renderScene(pRenderer* renderer);

	//end scripts attached to sceneObjects
	void endScene();

	//creates a new scene and adds it to the list
	void addScene();

	inline pScene* getCurrentScene()
	{
		//Return first scene if there is no current
		if (currentScene == nullptr) {
			if (sceneList.empty()) { sceneList.push_back(new pScene()); }
			currentScene = sceneList.back();
		}

		return currentScene;
	}

	inline bool hasCurrentScene() {
		return (currentScene != nullptr);
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

