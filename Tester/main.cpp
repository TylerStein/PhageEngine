#pragma once
#include "pModel.h"
#include "PhageEngine.h"

/*
//Placeholder hardcoded shaders
const char* vertex_shader =
"#version 400\n"
"in vec3 vp;"
"void main() {"
"  gl_Position = vec4(vp, 1.0);"
"}";

const char* fragment_shader =
"#version 400\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}";

//Create some points to render

*/

int main() {
	//Create the phage engine on the heap 
	//(Make this a singleton process later, we don't want to instantiate it like this)
	PhageEngine* phage = new PhageEngine();

	//Create the window
	phage->CreateWindow(800, 600, "PhageTest");

	//Make a triangle for the top-left
	GLuint testVertCount = 3;
	GLfloat testVerts[] = {
		-0.75f, 0.75f,  0.0f,
		-0.75f, 0.0f,  0.0f,
		0.0f, 0.0f,  0.0f
	};

	//Make a second triangle for the top-right
	GLuint otherVertCount = 3;
	GLfloat otherVerts[] = {
		0.75f, -0.75f,  0.0f,
		-0.0f, -0.75f,  0.0f,
		0.75f, 0.0f,  0.0f
	};


	//Make a blank placeholder material (currently does nothing)
	pMaterial* testMat = new pMaterial();

	//Make the models on the heap
	pModel* testModel = new pModel("TestModel", testMat, testVerts, testVertCount);
	pModel* otherModel = new pModel("SecondModel", testMat, otherVerts, otherVertCount);

	//Add the models to the model list
	phage->modelList.push_back(testModel);
	phage->modelList.push_back(otherModel);

	//Start the engine loop
	phage->Start();

	return 0;
}
