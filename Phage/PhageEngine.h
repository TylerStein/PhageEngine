#pragma once
#include <vector>
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "GameImplement.h"
#include "pRenderer.h"
#include "pResourceFactory.h"
#include "pSceneManager.h"
#include "pSoundSystem.h"
#include "pLightManager.h"
#include "pScriptManager.h"
#include "pInputManager.h"
#include "pAnimationManager.h"
#include "pSkeletonManager.h"

class PhageEngine
{
public:
	static PhageEngine* setupEngine(GameImplement* game) {
		if (instance == nullptr) {
			instance = new PhageEngine(game);
		}

		return instance;
	}

	static PhageEngine* GetInstance() {
		if (instance == nullptr) {
			LogManager::instance()->error("Attempted to get engine instance without a game implement!");
			instance = new PhageEngine(nullptr);
		}
		return instance;
	}

	PhageEngine(GameImplement* game);
	~PhageEngine();
	//Placeholder window creation method
	void CreateWindow(GLint width, GLint height, char* title);
	void Start();

	pRenderer* renderer;
	pResourceFactory* resourceFactory;
	pLightManager* lightManager;
	pSceneManager* sceneManager;
	pInputManager* inputManager;

	float windowWidth;
	float windowHeight;
	GLFWwindow* window;
private:
	static PhageEngine* instance;

	//Input requires being set up AFTER the window is ready
	void SetupInput();

	void doLoop();

	GameImplement* game;
	pModelManager* modelManager;
	pMaterialManager* materialManager;
	pImageManager* imageManager;
	pShaderManager* shaderManager;
	pAudioManager* audioManager;
	pSceneGraph* sceneGraph;
	pScriptManager* scriptManager;

	pAnimationManager* animationManager;
	pSkeletonManager* skeletonManager;

	pInputHandler* inputHandler;
};

