#pragma once
#include "PhageEngine.h"


int main() {
	//Create the phage engine on the heap 
	//(Make this a singleton process later, we don't want to instantiate it like this)
	PhageEngine* phage = new PhageEngine();

	//Create the window
	phage->CreateWindow(800, 600, "PhageTest");
	GLfloat vertices[8] = {
		-0.10f, -0.10f, // Square
		 0.1f, -0.1f,
		 0.10f, 0.1f,
		 -0.1f, 0.1f
	};

	char name = 'd';

	char* n = &name;

	pMaterial* material = new pMaterial();

	pModel* model = new pModel(n, material, vertices, 2);

	

	//Start the engine loop
	phage->Start();

	phage->renderer->renderModel(model);

	return 0;
}
