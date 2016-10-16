#pragma once
#include "PhageEngine.h"


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

	GLfloat testColors[] = {
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f
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
	pModel* testModel = new pModel("TestModel", testMat, testVerts, testColors, testVertCount);
	pModel* otherModel = new pModel("SecondModel", testMat, otherVerts, testColors, otherVertCount);

	//Add the models to the model list
	phage->modelList.push_back(testModel);
	phage->modelList.push_back(otherModel);

	//Start the engine loop
	phage->Start();

	delete phage;
	delete testMat;
	delete testModel;
	delete otherModel;

	return 0;
}
