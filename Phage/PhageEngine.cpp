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

	//Get version info, do with these what you will (probably print to log)
	const GLubyte* renderer = glGetString(GL_RENDERER); //Renderer version
	const GLubyte* version = glGetString(GL_VERSION); //OpenGL Version

	glEnable(GL_DEPTH_TEST); //Enable depth-testing
	glDepthFunc(GL_LESS); //Interpret smaller values as closer
}

void PhageEngine::Start()
{
	doLoop();
}

void PhageEngine::doLoop() {
	//While the window isn't being closed
	while (!glfwWindowShouldClose(window)) {
		//Run the application

	}
}