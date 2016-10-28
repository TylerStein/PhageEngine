#pragma once
#include "PhageEngine.h"
#include "TestScene.h"
#include <iostream>

TestScene* game;
PhageEngine* engine;
int windowWidth = 800;
int windowHeight = 600;
char* windowTitle = "PhageGame";


int main() {

	//Create a test scene to use the engine
	game = new TestScene();

	//Set up the engine and get the reference
	engine = PhageEngine::setupEngine(game);

	//Feed the engine pointer to the game
	game->giveEngineReference(engine);

	//Set the engine window
	engine->CreateWindow(windowWidth, windowHeight, windowTitle);
	
	//Start the engine loop
	engine->Start();

	return 0;
}

/*
//User-end engine use is through implementing the abstract GameImplement class and setting this up in main
class Game : public GameImplement {
public:
	pImage* testImg;
	pMaterial* testMat;
	pModel* testModel;

	Game() {

	}

	~Game() {
	
	}

	//Called on engine ready
	void onStart() {
		std::cout << "onStart()" << std::endl;

		//Make a triangle for the top-left
		GLuint testVertCount = 3;
		GLfloat testVerts[] = {
			0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f
		};

		GLfloat testColors[] = {
			1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f
		};

		GLfloat testUVs[] = {
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f
		};

		GLfloat testNormals[] = {
			1.0f, 1.0f,  1.0f,
			1.0f, 1.0f,  1.0f,
			1.0f, 1.0f,  1.0f
		};

		//Manually create some material info
		pMaterial::materialInfo mtl;
		mtl.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		mtl.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		mtl.alpha = 1.0f;
		mtl.shininess = 1.0f;
		mtl.bHasTexture = false;
		mtl.blendMode = GL_ADD;

		//Generate an image called TestImage
		testImg = engine->resourceFactory->createImage("TestImage", "../Resources/Images/bricks.png");

		//Generate a material called TestMat
		testMat = engine->resourceFactory->createMaterial("TestMat", testImg);

		//Generate a model called TestModel
		testModel = engine->resourceFactory->createModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount);

		//Set the renderer view matrix
		engine->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		engine->renderer->setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 5.0f);

		//Set the position of the model to 0, 0, 0
		testModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		//Set the rotation of the model Z to 0
		testModel->rotateAround(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	}

	//Called on engine closing
	void onEnd() {
		std::cout << "onEnd()" << std::endl;

		delete testImg;
		delete testMat;
		delete testModel;
	}

	//Called after render set, for non-render updates
	void onUpdate() {
		glfwPollEvents();

	}

	//Called after onUpdate for post-updat events
	void onPostUpdate() {

	}

	//Called right before rendering for render setup
	void onPreRender() {
		//Clear the drawing surface
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//Called before onUpdate for visual updates
	void onRender() {

	}

	//Called right after onUpdate for post-render actions
	void onPostRender() {
		glfwSwapBuffers(engine->window);
	}
};
*/
