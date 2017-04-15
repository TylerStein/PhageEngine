#include "TestScene.h"
#include "GLError.h"
#include "VisualDebugger.h"

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
	//Get a reference to the current scene
	pScene* scene = engine->sceneManager->getCurrentScene();

	//Create a camera
	pCamera* tmpCamera = new pCamera(engine->windowWidth, engine->windowHeight, 60.0f, 256.0f, 0.2f);

	//Set the renderer's projection properties using the camera's calculations
	engine->renderer->setProjectionMatrix(tmpCamera->getProjectionMatrix());

	//Put the camera in a scene object for transformations
	cameraObject = scene->createNewObject(nullptr, nullptr, nullptr, tmpCamera, glm::vec3(0, 0, -20), glm::vec3(0, 0, 0), glm::vec3(1), "Camera", nullptr);

	engine->renderer->setViewMatrix(cameraObject->getSceneNode()->getModelMatrix());

	//Create a shader for the shape surfaces
	pShader* phongShader = engine->resourceFactory->createPhongShader();

	pImage* bricksImageDiffuse = engine->resourceFactory->loadImage("Image_Bricks", "../Resources/Images/Museum/stonefloor.jpg");
	pImage* bricksImageNormal = engine->resourceFactory->loadImage("Image_Bricks_Norm", "../Resources/Images/Museum/stonefloor_norm.jpg");

	pMaterial* floorMaterial = engine->resourceFactory->createMaterial("Material_Floor", phongShader, MaterialInfo(
		glm::vec3(1), 
		glm::vec3(0.01), 
		glm::vec3(1), 
		glm::vec3(0), 
		64.0f, 
		bricksImageDiffuse,
		nullptr,
		bricksImageNormal,
		true
	));

	pSceneNode* floorNode = engine->resourceFactory->addPrimitiveToScene("Mesh_Floor", pPrimitiveMaker::PLANE_TRI, glm::vec3(1.0, 1.0, 1.0), glm::vec3(1), floorMaterial);
	pModel* floorMesh = floorNode->getAttachedSceneObject()->getAttachedModel();
	floorMesh->scaleTextureCoordinates(glm::vec2(6.0f, 6.0f));
	floorNode->setPosition(glm::vec3(0, 0, 0));
	floorNode->setRotation(glm::vec3(glm::radians(-90.0f), 0, 0));
	scene->addExistingNodes(floorNode);

	//pSceneNode* butterfly = engine->resourceFactory->loadModelToScene("Butterfly", "../Resources/Models/Butterfly/Butterfly.obj", *scene, scene->sceneGraph->getRootSceneNode());
	//pSceneNode* house = engine->resourceFactory->loadModelToScene("House", "../Resources/Models/House/house interior.obj", *scene, nullptr);
	//house->setScale(glm::vec3(0.1f));

	pSceneNode* soldier = engine->resourceFactory->loadModelToScene("Soldier", "../Resources/Models/ArmyPilot/ArmyPilot.fbx", *scene, nullptr);
	soldier->setScale(glm::vec3(0.01f));

	pCubeMap* skyboxCube = new pCubeMap("../Resources/Cubemaps/Citadella/posx.jpg",
		"../Resources/Cubemaps/Citadella/negx.jpg",
		"../Resources/Cubemaps/Citadella/posy.jpg",
		"../Resources/Cubemaps/Citadella/negy.jpg",
		"../Resources/Cubemaps/Citadella/posz.jpg",
		"../Resources/Cubemaps/Citadella/negz.jpg"
		);
	engine->renderer->setSkybox(skyboxCube);

	//Add a light to the game (should be via scene)
	engine->lightManager->addLight(new pLight(Light::DIRECTIONAL, glm::vec3(0.5, 0, 0.5), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0.0f)));
	//engine->lightManager->addLight(new pLight(Light::POINT, glm::vec3(10, 10, 0), 26.0f, glm::vec3(1.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 1.0), glm::vec3(0.0f)));
}

void TestScene::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TestScene::onRender()
{
	engine->renderer->renderSkybox();
}

void TestScene::onPostRender()
{
	glfwSwapBuffers(engine->window);
}

void TestScene::onUpdate(GLdouble deltaTime)
{
	//pSceneNode* chairNode = chairObject->getSceneNode();
	pSceneNode* camNode = cameraObject->getSceneNode();

	//camNode->setForwardVector();
	//camNode->setRightVector();
	//camNode->setUpVector();

	const float move = 1.5f * deltaTime;
	const float rot = 0.3f * deltaTime;

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_W)) {
		camNode->translate(camNode->getForwardVector() * move);
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_S)) {
		camNode->translate(camNode->getForwardVector() * -move);
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_D)) {
		camNode->translate(camNode->getRightVector() * -move);
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_A)) {
		camNode->translate(camNode->getRightVector() * move);
	}
	
	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_E)) {
		camNode->translate(glm::vec3(0, -move, 0));
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_Q)) {
		camNode->translate(glm::vec3(0, move, 0));
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_RIGHT)) {
		camNode->rotateAround(glm::vec3(0, 1, 0), rot);
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_LEFT)) {
		camNode->rotateAround(glm::vec3(0, 1, 0), -rot);
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_UP)) {
		camNode->rotateAround(camNode->getRightVector(), -rot);
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_DOWN)) {
		camNode->rotateAround(camNode->getRightVector(), rot);
	}

	engine->renderer->setViewMatrix(camNode->getModelMatrix());
}

void TestScene::onPostUpdate()
{
}

void TestScene::onEnd()
{

}

