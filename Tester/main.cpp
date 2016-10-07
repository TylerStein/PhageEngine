#pragma once
#include "pModel.h"
#include "PhageEngine.h"


int main() {
	//Create the phage engine on the heap 
	//(Make this a singleton process later, we don't want to instantiate it like this)
	PhageEngine* phage = new PhageEngine();

	//Create the window
	phage->CreateWindow(800, 600, "PhageTest");

	//Start the engine loop
	phage->Start();

	return 0;
}
