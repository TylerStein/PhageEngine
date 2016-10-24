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
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f
	};

	GLfloat testUVs[] = {
		-0.75f, 0.75f,
		-0.75f, 0.0f,
		0.0f, 0.0f
	};

	GLfloat testNormals[] = {
		0.0f, 0.0f,  1.0f,
		0.0f, 0.0f,  1.0f,
		0.0f, 0.0f,  1.0f
	};

	//Manually material info
	pMaterial::materialInfo mtl;
	mtl.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	mtl.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	mtl.alpha = 1.0f;
	mtl.shininess = 1.0f;
	mtl.bHasTexture = false;
	mtl.blendMode = GL_ADD;

	//Generate a material called TestMat
	pMaterial* testMat = phage->resourceFactory->createMaterial("TestMat", mtl);

	//Generate a model called TestModel
	pModel* testModel = phage->resourceFactory->createModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount);

	printf("Material name is: %s\n", phage->resourceFactory->getMaterial("TestMat")->getName());
	printf("Model name is: %s\n", phage->resourceFactory->getModel("TestModel")->getName());

	printf("Material shader id %i\n", phage->resourceFactory->getMaterial("TestMat")->getShaderProgramID());
	printf("Model vert count %i\n", phage->resourceFactory->getModel("TestModel")->getVertCount());

	//Make an image
	//pImage* testImage = new pImage("Bricks", "../Resources/Images/bricks.png");

	//Make a material
	//pMaterial* testMat = new pMaterial("TestMat", testImage);
	//pMaterial* testMat = new pMaterial("TestMat", glm::vec3(1.0f, 1.0f, 1.0f));

	//Make the models on the heap
	//pModel* testModel = new pModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount);

	//Clear the vert info and material from memory (they've been copied over to the models)
	//delete testMat;
	//delete[] testVerts;
	//delete[] testNormals;
	//delete[] testColors;
	//delete[] testUVs;

	//testModel->rotateAround(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	//testModel->setScale(glm::vec3(2.0f, 2.0f, 1.0f));
	//testModel->translate(glm::vec3(0.0f, 0.0f, 0.0f));

	phage->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Add the models to the model list
	//phage->modelList.push_back(testModel);
	//phage->modelList.push_back(otherModel);

	//Start the engine loop
	phage->Start();

	//delete testImage;
	delete testMat;
	delete testModel;
	delete phage;


	return 0;
}
