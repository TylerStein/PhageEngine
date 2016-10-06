#pragma once
#include "GL\glew.h"
#include "GLFW\glfw3.h"

/*Class within the library to act as a palceholder in early development*/

class PhageEngine
{
public:
	PhageEngine();
	~PhageEngine();

	//Placeholder window creation method
	void CreateWindow(GLint width, GLint height, char* title);
	void Start();

private:
	void doLoop();
	GLFWwindow* window;
};

