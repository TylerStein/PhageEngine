#pragma once
#include <vector>
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "pRenderer.h"
#include "pResourceFactory.h"
#include "GameImplement.h"
#include "pLightManager.h"

class PhageEngine
{
public:
	static PhageEngine* setupEngine(GameImplement* game) {
		if (instance == nullptr) {
			instance = new PhageEngine(game);
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

	std::vector<pModel*> modelList;

	float windowWidth;
	float windowHeight;
	GLFWwindow* window;
private:
	static PhageEngine* instance;

	void doLoop();

	GameImplement* game;
	pModelManager* modelManager;
	pMaterialManager* materialManager;
	pImageManager* imageManager;
	pShaderManager* shaderManager;
};

