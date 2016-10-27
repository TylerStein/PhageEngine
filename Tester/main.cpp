#pragma once
#include "PhageEngine.h"
#include <iostream>

int main() {
	//Create the phage engine on the heap 
	//(Make this a singleton process later, we don't want to instantiate it like this)
	PhageEngine* phage = new PhageEngine();

	//Create the window
	phage->CreateWindow(800, 600, "PhageTest");	

	//Make a triangle for the top-left
	GLuint testVertCount = 3;
	GLfloat testVerts[] = {
		-1.0f, 1.0f,  0.0f,
		-1.0f, 0.0f,  0.0f,
		0.0f, 0.0f,  0.0f
	};

	GLfloat testColors[] = {
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f
	};

	GLfloat testUVs[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	GLfloat testNormals[] = {
		1.0f, 1.0f,  1.0f,
		1.0f, 1.0f,  1.0f,
		1.0f, 1.0f,  1.0f
	};

	//Manually create some material info
	pMaterial::materialInfo mtl;
	mtl.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	mtl.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	mtl.alpha = 1.0f;
	mtl.shininess = 1.0f;
	mtl.bHasTexture = false;
	mtl.blendMode = GL_ADD;

	//Generate an image called TestImage
	pImage* testImg = phage->resourceFactory->createImage("TestImage", "../Resources/Images/bricks.png");

	//Generate a material called TestMat
	pMaterial* testMat = phage->resourceFactory->createMaterial("TestMat", testImg);

	//Generate a model called TestModel
	pModel* testModel = phage->resourceFactory->createModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount);
	pModel* otherModel = phage->resourceFactory->createModel("OtherModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount);

	//Test grabbing resources from managers via the factory
	std::cout << "Material name is: " << phage->resourceFactory->getMaterial("TestMat")->getName() << std::endl;
	std::cout << "First model name is: " << phage->resourceFactory->getModel("TestModel")->getName() << std::endl;
	std::cout << "Other model name is: " << phage->resourceFactory->getModel("OtherModel")->getName() << std::endl;
	
	//Set the renderer view matrix
	phage->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	phage->renderer->setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 5.0f);

	//Add the models to the model list - eventually will be replaced by scene graph
	phage->modelList.push_back(testModel);
	phage->modelList.push_back(otherModel);

	//Mess with the model matrices
	testModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	otherModel->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));

	testModel->rotateAround(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(45.0f));

	//Start the engine loop
	phage->Start();

	//delete testImage;
	delete testMat;
	delete testModel;
	delete otherModel;
	delete phage;


	return 0;
}
