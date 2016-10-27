#include "PhageEngine.h"
#include <iostream>

PhageEngine* PhageEngine::instance = NULL;

PhageEngine::PhageEngine(GameImplement* game)
{
	//Attempt to initialize GLFW
	if (!glfwInit()) {
		//GLFW failed to initialize
		
		printf("GLFW INIT ERROR");
	}


	this->game = game;

	//Create an instance of the managers
	modelManager = new pModelManager();
	materialManager = new pMaterialManager();
	imageManager = new pImageManager();

	//Give the modelmanager the other managers
	modelManager->setMaterialManager(materialManager);
	modelManager->setImageManager(imageManager);

	//Create an instance of the factory and give it the managers
	resourceFactory = new pResourceFactory();
	resourceFactory->setModelManager(modelManager);
	resourceFactory->setMaterialManager(materialManager);
	resourceFactory->setImageManager(imageManager);
}


PhageEngine::~PhageEngine()
{	
	//Destroy the window
	glfwDestroyWindow(window);

	//End glfw processes
	glfwTerminate();

	//Destruct the renderer
	renderer->~pRenderer();
	delete renderer;
	
	//CLEARING KEYS AND VALUES CAUSES AN ERROR!!!
	delete modelManager;
	delete materialManager;
	delete imageManager;
	delete resourceFactory;

	//Delete managers and factory
	//modelManager->~pModelManager();
	//materialManager->~pMaterialManager();
	//imageManager->~pImageManager();
	//resourceFactory->~pResourceFactory();
}

void PhageEngine::CreateWindow(GLint width, GLint height, char* title)
{
	//Create the window (width, height, windowTitle, monitor, share*)
	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		//Window or OpenGL context creation failed
	}

	//Set the preferred OpenGL version to 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//Set the minimum supported OpenGL version to 3.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Set the context to the current window
	glfwMakeContextCurrent(window);

	//Start GLEW
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		//glew failed to initialize
		printf("GLEW INIT ERROR");

	}

	//Enable gl debug messages
	glEnable(GL_DEBUG_OUTPUT);


	//Set up input mode
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Get version info, do with these what you will (probably print to log)
	const GLubyte* renderVersion = glGetString(GL_RENDERER); //Renderer version
	const GLubyte* glVersion = glGetString(GL_VERSION); //OpenGL Version

	printf("Renderer: %s\n", renderVersion);
	printf("Supported OGL Version: %s\n", glVersion);

	renderer = new pRenderer(window);
}

void PhageEngine::Start()
{
	//Start the loop!
	doLoop();
}

//Update for pre-rendering events called every frame
void PhageEngine::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Update for rendering events called every frame
void PhageEngine::onRender()
{
	if (!modelList.empty()) {
		for (int i(0); i < modelList.size(); ++i) {
			renderer->renderModel(modelList.at(i));
		}
	}
}

//Update for events called after render every frame
void PhageEngine::onPostRender()
{
	glfwSwapBuffers(window);
}

//Function for update events called every frame
void PhageEngine::onUpdate()
{
	glfwPollEvents();

}

//Function for post-update events every frame
void PhageEngine::onPostUpdate()
{
}

void PhageEngine::doLoop() {
	//While the window isn't being closed, call the looped functions
	game->onStart();
	do {
		game->onUpdate();
		game->onPostUpdate();
		game->onPreRender();
		game->onRender();
		game->onPostRender();
		//onUpdate();
		//onPostUpdate();
		//onPreRender();
		//onRender();
		//onPostRender();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	this->~PhageEngine();
	game->onEnd();
}