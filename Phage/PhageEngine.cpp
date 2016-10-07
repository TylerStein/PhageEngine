#include "PhageEngine.h"

PhageEngine::PhageEngine()
{
	//Attempt to initialize GLFW
	if (!glfwInit()) {
		//GLFW failed to initialize
		
	}
}


PhageEngine::~PhageEngine()
{
	//Destruct the renderer
	renderer->~pRenderer();
	
	//Destroy the window
	glfwDestroyWindow(window);

	//End glfw processes
	glfwTerminate();
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
	glewInit();

	//Set up input mode
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Get version info, do with these what you will (probably print to log)
	const GLubyte* renderVersion = glGetString(GL_RENDERER); //Renderer version
	const GLubyte* glVersion = glGetString(GL_VERSION); //OpenGL Version

	renderer = new pRenderer(window);
}

void PhageEngine::Start()
{
	doLoop();
}

void PhageEngine::doLoop() {
	//While the window isn't being closed, loop here
	do {

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}