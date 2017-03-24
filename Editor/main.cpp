#pragma once
#include "PhageEngine.h"
#include "DebugEditor.h"

PhageEngine* engine;
DebugEditor* editor;
int windowWidth = 1920;
int windowHeight = 1080;
char* windowTitle = "Phage Engine Editor";


int main() {

	//Create a test scene to use the engine
	editor = new DebugEditor();

	//Set up the engine and get the reference
	engine = PhageEngine::setupEngine(editor);

	//Feed the engine pointer to the game
	editor->giveEngineReference(engine);

	//Set the engine window
	engine->CreateWindow(windowWidth, windowHeight, windowTitle);

	//Start the engine loop
	engine->Start();

	return 0;
}

