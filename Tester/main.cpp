#pragma once
#include "PhageEngine.h"
#include "TestScene.h"
#include <iostream>

TestScene* game;
PhageEngine* engine;
int windowWidth = 800;
int windowHeight = 800;
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

