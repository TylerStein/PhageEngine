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

	GLuint planeVertCount = 6;
	//Make a plane for the floor
	GLfloat planeVerts[] = {
		-1, -1, 0,
		-1, 1, 0,
		1, -1, 0,

		1, -1, 0,
		-1, 1, 0,
		1, 1, 0
	};
	GLfloat planeNormals[] = {
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1
	};
	GLfloat planeUVs[] = {
		0, 0,
		0, 1,
		1, 0,

		1, 0,
		0, 1,
		1, 1
	};
	GLfloat planeColors[] = {
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0
	};

	GLfloat cubeVertCount = 36;
	//Create temporary arrays of information representing a 3D cube
	GLfloat cubeVerts[] = {
		//front
		-1, 1, 1,
		-1, -1, 1,
		1, -1, 1,

		-1, 1, 1,
		1, 1, 1,
		1, -1, 1,

		//right side
		1, -1, 1,
		1, -1, -1,
		1, 1, -1,

		1, -1, 1,
		1, 1, 1,
		1, 1, -1,

		//top
		1, 1, -1,
		-1, 1, -1,
		1, 1, 1,

		1, 1, 1,
		-1, 1, 1,
		-1, 1, -1,

		//back
		-1, 1, -1,
		1, 1, -1,
		1, -1, -1,

		1, -1, -1,
		-1, -1, -1,
		-1, 1, -1,

		//left side
		-1, 1, -1,
		-1, 1, 1,
		-1, -1, 1,

		-1, -1, 1,
		-1, 1, -1,
		-1, -1, -1,

		//bottom
		-1, -1, -1,
		1, -1, -1,
		-1, -1, 1,

		-1, -1, 1,
		1, -1, 1,
		1, -1, -1
	};
	GLfloat cubeColors[] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};
	GLfloat cubeTexCoords[] = {
		//front
		0, 1,
		0, 0,
		1, 0,

		0, 1,
		1, 1,
		1, 0,

		//right side
		0, 1,
		0, 0,
		1, 0,

		0, 1,
		1, 1,
		1, 0,

		//top
		1, 0,
		0, 0,
		1, 1,

		1, 1,
		0, 1,
		0, 0,

		//back
		0, 1,
		1, 1,
		1, 0,

		1, 0,
		0, 0,
		0, 1,

		//left side
		1, 0,
		1, 1,
		0, 1,

		0, 1,
		1, 0,
		0, 0,

		//bottom
		0, 0,
		1, 0,
		0, 1,

		0, 1,
		1, 1,
		1, 0

	};
	GLfloat cubeNormals[] = {
		//front
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		//right side
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,

		//top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		//back
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		//left side
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		//bottom
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
	};


	//Print controls
	std::cout << "Controls:\nMove cube with WASD, Q/E for Z movement\nMove light with IJKL, U/O for Z movement\n\n";

	//Create the shader attributes and uniforms to link with the shader being used
	uniformNameMap uniMap = uniformNameMap();
	attribNameMap atrMap = attribNameMap();
	//Brackets so this section can be collapsed
	{
		atrMap.insert(Attributes::VertexPosition, "vPosition");
		atrMap.insert(Attributes::VertexCoordinate, "vTexCoord");
		atrMap.insert(Attributes::VertexNormal, "vNormal");

		uniMap.insert(Uniforms::Camera_View, "cameraView");
		uniMap.insert(Uniforms::Projection_View, "projView");
		uniMap.insert(Uniforms::Model_View, "modelView");

		uniMap.insert(Uniforms::DiffuseColor, "fDiffuse");
		uniMap.insert(Uniforms::AmbientColor, "fAmbient");
		uniMap.insert(Uniforms::DiffuseTexture, "fTexture");
		uniMap.insert(Uniforms::SpecularColor, "fSpecular");
		uniMap.insert(Uniforms::Shininess, "fShininess");
		uniMap.insert(Uniforms::Emission, "fEmit");

		uniMap.insert(Uniforms::Light_Position, "lPosition");
		uniMap.insert(Uniforms::Light_Color, "lColor");
		uniMap.insert(Uniforms::Light_Power, "lPower");
		uniMap.insert(Uniforms::Light_Ambient, "lAmbient");
		uniMap.insert(Uniforms::Light_Attenuation, "lAttenuation");
		uniMap.insert(Uniforms::Light_Range, "lRange");
		uniMap.insert(Uniforms::Light_Count, "lightCount");
	}
	//Create a shader for the shape surfaces
	pShader* tmpPhongShaderRef = engine->resourceFactory->createShader("Phong_Lighting", atrMap, uniMap, "../Resources/Shaders/phong2.vert", "../Resources/Shaders/phong2.frag");

	//Generate an image for the crate diffuse and bump maps
	engine->resourceFactory->createImage("Crate_Diffuse", "../Resources/Images/Crate/crate1_diffuse.png");
	engine->resourceFactory->createImage("Chair_Diffuse", "../Resources/Models/chair/diffuse.tga");
	engine->resourceFactory->createImage("Barrel_Diffuse", "../Resources/Models/Oildrum/oildrum_diffuse.jpg");
	engine->resourceFactory->createImage("Floor_Diffuse", "../Resources/Images/Floor/diffuse.tga");

	//Manually create some material info for the crate and floor
	MaterialInfo crateMaterial;
	crateMaterial.reset();
	crateMaterial.diffuse = glm::vec3(1);
	crateMaterial.ambient = glm::vec3(0);
	crateMaterial.specular = glm::vec3(0.6f);
	crateMaterial.shininess = 1.2f;
	crateMaterial.diffuseTexture = engine->resourceFactory->getImage("Crate_Diffuse");

	MaterialInfo chairMaterial;
	chairMaterial.reset();
	chairMaterial.diffuseTexture = engine->resourceFactory->getImage("Chair_Diffuse");

	MaterialInfo barrelMaterial;
	barrelMaterial.reset();
	barrelMaterial.diffuseTexture = engine->resourceFactory->getImage("Barrel_Diffuse");

	MaterialInfo floorMaterial;
	floorMaterial.reset();
	floorMaterial.diffuse = glm::vec3(1);
	floorMaterial.ambient = glm::vec3(0);
	floorMaterial.specular = glm::vec3(0.6f);
	floorMaterial.shininess = 1.2f;
	floorMaterial.diffuseTexture = engine->resourceFactory->getImage("Floor_Diffuse");

	MaterialInfo lightModelMaterial;
	lightModelMaterial.reset();
	lightModelMaterial.useLight = false;
	lightModelMaterial.diffuse = glm::vec3(1.0, 0.0, 0.0);

	//Generate a material for the crate using the shader and material info created
	pMaterial* tmpCrateMatRef = engine->resourceFactory->createMaterial("Crate_Material", tmpPhongShaderRef, crateMaterial);
	pMaterial* tmpChairMatRef = engine->resourceFactory->createMaterial("Chair_Material", tmpPhongShaderRef, chairMaterial);
	pMaterial* tmpBarrelMatRef = engine->resourceFactory->createMaterial("Barrel_Material", tmpPhongShaderRef, barrelMaterial);
	pMaterial* tmpFloorMatRef = engine->resourceFactory->createMaterial("Floor_Material", tmpPhongShaderRef, floorMaterial);
	pMaterial* tmpLightMatRef = engine->resourceFactory->createMaterial("LightSrc_Material", engine->resourceFactory->createDebugShader(), lightModelMaterial);

	//Create a box and set its material to look like a wooden crate
	crateRef = engine->resourceFactory->createPrimitiveShape("Crate_Model", pPrimitiveMaker::CUBOID_TRI, glm::vec3(1), glm::vec3(1), tmpCrateMatRef);
	//crateRef = engine->resourceFactory->createModel("Crate_Model", tmpCrateMatRef, cubeVerts, cubeNormals, cubeColors, cubeTexCoords, cubeVertCount, GL_TRIANGLES);
	//crateRef = engine->resourceFactory->createModel("Crate_Model", tmpCrateMatRef, planeVerts, planeNormals, planeColors, planeUVs, planeVertCount, GL_TRIANGLES);
	chairRef = engine->resourceFactory->loadModel("Chair_Model", "../Resources/Models/chair/chair.obj", tmpChairMatRef);
	barrelRef = engine->resourceFactory->loadModel("Barrel_Model", "../Resources/Models/Oildrum/oildrum.obj", tmpBarrelMatRef);

	//Create a plane to act as the floor and give it the floor material
	floorRef = engine->resourceFactory->createModel("Floor_Model", tmpFloorMatRef, planeVerts, planeNormals, planeColors, planeUVs, planeVertCount, GL_TRIANGLES);
	
	//Create the light models (for visualisation of their locations)
	mdl_LightA = engine->resourceFactory->createModel("LightSrc_A_Model", tmpLightMatRef, cubeVerts, cubeNormals, cubeColors, cubeTexCoords, cubeVertCount, GL_TRIANGLES);
	mdl_LightB = engine->resourceFactory->createModel("LightSrc_B_Model", tmpLightMatRef, cubeVerts, cubeNormals, cubeColors, cubeTexCoords, cubeVertCount, GL_TRIANGLES);
	mdl_LightC = engine->resourceFactory->createModel("LightSrc_C_Model", tmpLightMatRef, cubeVerts, cubeNormals, cubeColors, cubeTexCoords, cubeVertCount, GL_TRIANGLES);

	//Position and angle the camera view (this should be done in its own class eventually)
	engine->renderer->setViewMatrix(glm::vec3(0.0f, 5.0f, 13.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//This overload of setProjectionMatrix() takes values for FOV
	engine->renderer->setProjectionMatrix(80, 1200, 1200, 0.2f, 120);

	//Move the models around to set up the scene initially
	crateRef->setPosition(glm::vec3(-2.0f, 1.0f, -2.0f));
	crateRef->setScale(glm::vec3(0.5f));

	chairRef->setPosition(glm::vec3(0.0f, 1.0f, -2.0f));
	chairRef->setScale(glm::vec3(0.5f));

	barrelRef->setPosition(glm::vec3(1.75f, 0.75f, 2.0f));
	barrelRef->setScale(glm::vec3(2.0f));

	floorRef->scale(glm::vec3(2.0f, 2.0f, 2.0f));
	floorRef->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	floorRef->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	floorRef->setScale(glm::vec3(8.0f, 8.0f, 1.0f));

	//Create 3 colored lights in the scene				Type					Position						Range	Color					Intensity		Ambient				Attenuation
	light_A = engine->lightManager->addLight(new pLight(Light::LightType::POINT, glm::vec3(-4.0f, 2.0f, 0.0f), 42, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), 2.0f));
	light_B = engine->lightManager->addLight(new pLight(Light::LightType::POINT, glm::vec3(4.0f, 2.0f, 0.0f), 42, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f), glm::vec3(0.0f), 2.0f));
	light_C = engine->lightManager->addLight(new pLight(Light::LightType::POINT, glm::vec3(0.0f, 2.0f, 4.0f), 42, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f), glm::vec3(0.0f), 2.0f));

	//Set the light model positions
	mdl_LightA->setPosition(light_A->getPosition());
	mdl_LightB->setPosition(light_B->getPosition());
	mdl_LightC->setPosition(light_C->getPosition());

	//Scale the light models
	mdl_LightA->setScale(glm::vec3(0.1f));
	mdl_LightB->setScale(glm::vec3(0.1f));
	mdl_LightC->setScale(glm::vec3(0.1f));

	lastMouseX = 0;
	lastMouseY = 0;

	LogManager::instance()->warning("Here be the log manager.");

	//play the sound
	soundSystem->playSound();

	sceneNode = new pSceneNode();
	sceneGraph = new pSceneGraph();
	sceneObject = new pSceneObject("testObject");
	sceneGraph->renderSceneGraph(sceneNode, engine->renderer);
}

void TestScene::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TestScene::onRender()
{
	//Render the light models and update them to match their corresponding light
	mdl_LightA->getMaterial()->setDiffuseColor(light_A->getColor(), false);
	engine->renderer->renderModel(mdl_LightA);

	mdl_LightB->getMaterial()->setDiffuseColor(light_B->getColor(), false);
	engine->renderer->renderModel(mdl_LightB);

	mdl_LightC->getMaterial()->setDiffuseColor(light_C->getColor(), false);
	engine->renderer->renderModel(mdl_LightC);

	//Render floor and crate
	engine->renderer->renderModel(floorRef);
	engine->renderer->renderModel(crateRef);
	engine->renderer->renderModel(chairRef);
	engine->renderer->renderModel(barrelRef);
}

void TestScene::onPostRender()
{
	glfwSwapBuffers(engine->window);
}

void TestScene::onUpdate(GLdouble deltaTime)
{
	//Light models follow light sources
	mdl_LightA->setPosition(light_A->getPosition());
	mdl_LightB->setPosition(light_B->getPosition());
	mdl_LightC->setPosition(light_C->getPosition());

	if (glfwGetKey(engine->window, GLFW_KEY_R)) {
		floorRef->getMaterial()->getShader()->compileShader();
	}

	if (glfwGetKey(engine->window, GLFW_KEY_T)) {
		mdl_LightA->getMaterial()->getShader()->compileShader();
	}

	//Cube movement
	if (glfwGetKey(engine->window, GLFW_KEY_W)) {
		chairRef->translate(glm::vec3(0, 0.5f * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_S)) {
		chairRef->translate(glm::vec3(0, -0.5f * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_A)) {
		chairRef->translate(glm::vec3(-1 * deltaTime, 0, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_D)) {
		chairRef->translate(glm::vec3(1 * deltaTime, 0, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_Q)) {
		chairRef->translate(glm::vec3(0.0f, 0.0f, 1.0f * deltaTime));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_E)) {
		chairRef->translate(glm::vec3(0.0f, 0.0f, -1.0f * deltaTime));
	}

	double lastX = lastMouseX;
	double lastY = lastMouseY;
	glfwGetCursorPos(engine->window, &lastMouseX, &lastMouseY);

	double diffX = lastMouseX - lastX;
	double diffY = lastMouseY - lastY;

	if (glfwGetMouseButton(engine->window, GLFW_MOUSE_BUTTON_1)) {
		chairRef->rotateAround(glm::vec3(0, 1, 0), diffX * deltaTime);
		chairRef->rotateAround(glm::vec3(1, 0, 0), diffY * deltaTime);
	}

	testModel->rotateAround(glm::vec3(0, 1, 0), 5 * deltaTime);

	//sound update method required to run
	soundSystem->update();

	glfwPollEvents();
}

void TestScene::onPostUpdate()
{
}

void TestScene::onEnd()
{
	std::cout << "onEnd()" << std::endl;

	delete floorRef;
	delete crateRef;

	delete mdl_LightA;
	delete mdl_LightB;
	delete mdl_LightC;

	delete light_A;
	delete light_B;
	delete light_C;

}
