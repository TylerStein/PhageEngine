#include "TestScene.h"
#include "GLError.h"

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

	//Generate an image for the chair
	pImage* chairDiffuseImage = engine->resourceFactory->loadImage("Chair_Diffuse", "../Resources/Models/chair/diffuse.tga");
	pImage* floorDiffuseImage = engine->resourceFactory->loadImage("Floor_Diffuse", "../Resources/Images/Museum/stonefloor.jpg");

	//Manually create some material info for the chair
	MaterialInfo chairMaterialInfo;
	chairMaterialInfo.reset();
	chairMaterialInfo.ambient = glm::vec3(0.05f);
	chairMaterialInfo.specular = glm::vec3(1.0f);
	chairMaterialInfo.shininess = 1.0f;
	chairMaterialInfo.diffuseTexture = chairDiffuseImage;

	MaterialInfo floorMaterialInfo;
	floorMaterialInfo.reset();
	floorMaterialInfo.ambient = glm::vec3(0.05f);
	floorMaterialInfo.specular = glm::vec3(1.0f);
	floorMaterialInfo.shininess = 1.0f;
	floorMaterialInfo.diffuseTexture = floorDiffuseImage;

	//Generate the material for the chair
	pMaterial* chairMaterial = engine->resourceFactory->createMaterial("Chair_Material", phongShader, chairMaterialInfo);
	pMaterial* floorMaterial = engine->resourceFactory->createMaterial("Floor_Material", phongShader, floorMaterialInfo);

	pModel* bunnyRef = engine->resourceFactory->loadModel("bunnyModel", "../Resources/Models/bunny.obj");

	pSceneObject* rootBunny = scene->createNewObject(bunnyRef, nullptr, nullptr, nullptr, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(5), "RootBunny", nullptr);

	pSceneObject* childBunny1 = scene->createNewObject(bunnyRef, nullptr, nullptr, nullptr, glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(1), "RootBunny", rootBunny->getSceneNode());

	pSceneObject* childBunny2 = scene->createNewObject(bunnyRef, nullptr, nullptr, nullptr, glm::vec3(0, 0, -2), glm::vec3(0, 0, 0), glm::vec3(1), "RootBunny", rootBunny->getSceneNode());

	rootBunny->getSceneNode()->translate(glm::vec3(1, 0, 0));
	rootBunny->getSceneNode()->rotateAround(rootBunny->getSceneNode()->getUpVector(), 90);
	rootBunny->getSceneNode()->setScale(glm::vec3(10));

	//Load in model node tree and attach to scene root
	//pSceneNode* chairNode = pModelLoader::instance()->loadModelToSceneObjects("../Resources/Models/bunny.obj");
	//scene->addExistingNodes(chairNode);
	//chairNode->setScale(glm::vec3(5.0f));
	//chairNode->setPosition(glm::vec3(0, 2, 0));

	//pSceneNode* gunModel2 = pModelLoader::instance()->loadModelToSceneObjects("../Resources/Models/bunny.obj");
	//chairNode->appendChild(gunModel2);

	//gunModel2->translate(glm::vec3(3, 3, 3));
	//gunModel2->rotateAround(gunModel2->getUpVector(), -45);
	//gunModel2->setScale(glm::vec3(4.0f));

	//pSceneNode* gunModel3 = pModelLoader::instance()->loadModelToSceneObjects("../Resources/Models/bunny.obj");
	//chairNode->appendChild(gunModel3);

	//gunModel3->translate(glm::vec3(-3, -3, -3));
	//gunModel3->rotateAround(gunModel2->getUpVector(), 45);
	//gunModel3->setScale(glm::vec3(4.0f));

	pSceneNode* floorNode = engine->resourceFactory->addPrimitiveToScene("FloorShape", pPrimitiveMaker::CUBOID_TRI, glm::vec3(4.0, 1.0f, 4.0), glm::vec3(1), floorMaterial);
	floorNode->setPosition(glm::vec3(0, 0, 0));
	scene->addExistingNodes(floorNode);

	pCubeMap* skyboxCube = new pCubeMap("../Resources/Cubemaps/Citadella/posx.jpg",
		"../Resources/Cubemaps/Citadella/negx.jpg",
		"../Resources/Cubemaps/Citadella/posy.jpg",
		"../Resources/Cubemaps/Citadella/negy.jpg",
		"../Resources/Cubemaps/Citadella/posz.jpg",
		"../Resources/Cubemaps/Citadella/negz.jpg"
		);
	engine->renderer->setSkybox(skyboxCube);

	//chairNode->translate(glm::vec3(3, 3, 3));
	//chairNode->setScale(glm::vec3(0.5));
	//chairNode->rotateAround(chairNode->getUpVector(), 45);


	//Add a light to the game (should be via scene)
	engine->lightManager->addLight(new pLight(Light::DIRECTIONAL, glm::vec3(0.5, 0.5, 0.5), glm::vec3(1), glm::vec3(1), glm::vec3(0.1f)));
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
		camNode->rotateAround(camNode->getUpVector(), rot);
	}

	if (engine->inputManager->hasKeyBeenRepeated(keyName::KEY_LEFT)) {
		camNode->rotateAround(camNode->getUpVector(), -rot);
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

