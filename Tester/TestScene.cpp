#include "TestScene.h"



TestScene::TestScene()
{

}


TestScene::~TestScene()
{
}

void TestScene::giveEngineReference(PhageEngine * engine)
{
	this->engine = engine;
}

void TestScene::onStart()
{
	std::cout << "onStart()" << std::endl;

	//Make a triangle for the top-left
	GLuint testVertCount = 3;
	GLfloat testVerts[] = {
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
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
	testImg = engine->resourceFactory->createImage("TestImage", "../Resources/Images/bricks.png");

	//Generate a material called TestMat
	testMat = engine->resourceFactory->createMaterial("TestMat", testImg);

	//Generate a model called TestModel
	testModel = engine->resourceFactory->createModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount);

	//Set the renderer view matrix
	engine->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	engine->renderer->setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 5.0f);

	//Set the position of the model to 0, 0, 0
	testModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	//Set the rotation of the model Z to 0
	testModel->rotateAround(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
}

void TestScene::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void TestScene::onRender()
{
	engine->renderer->renderModel(testModel);
}

void TestScene::onPostRender()
{
	glfwSwapBuffers(engine->window);
}

void TestScene::onUpdate()
{
	glfwPollEvents();
}

void TestScene::onPostUpdate()
{
}

void TestScene::onEnd()
{
	std::cout << "onEnd()" << std::endl;

	delete testImg;
	delete testMat;
	delete testModel;
}
