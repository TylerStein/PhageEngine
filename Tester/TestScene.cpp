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
		uniMap.insert(Uniforms::Normal_View, "normalView");

		uniMap.insert(Uniforms::Camera_Position, "cameraLocation");

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
	pShader* tmpPhongShaderRef = engine->resourceFactory->createShader("Phong_Lighting", atrMap, uniMap, "../Resources/Shaders/PhongMaterial.vert", "../Resources/Shaders/PhongMaterial.frag");

	//Generate an image for the crate diffuse and bump maps
	engine->resourceFactory->createImage("Chair_Diffuse", "../Resources/Models/chair/diffuse.tga");
	engine->resourceFactory->createImage("Chair_Normal", "../Resources/Models/chair/normal.tga");
	engine->resourceFactory->createImage("Chair_Specular", "../Resources/Models/chair/specular.tga");

	engine->resourceFactory->createImage("Box_Diffuse", "../Resources/Images/Floor/diffuse.tga");

	engine->resourceFactory->createImage("Brick_Diffuse", "../Resources/Images/bricks.png");

	//Manually create some material info for the crate and floor
	MaterialInfo chairMaterial;
	chairMaterial.reset();
	chairMaterial.ambient = glm::vec3(0.05);
	chairMaterial.specular = glm::vec3(1);
	chairMaterial.shininess = 1;
	chairMaterial.diffuseTexture = engine->resourceFactory->getImage("Chair_Diffuse");

	MaterialInfo lightModelMaterial;
	lightModelMaterial.reset();
	lightModelMaterial.useLight = false;

	MaterialInfo floorMaterial;
	floorMaterial.reset();
	floorMaterial.ambient = glm::vec3(0.05);
	floorMaterial.specular = glm::vec3(1);
	floorMaterial.shininess = 1;
	floorMaterial.diffuseTexture = engine->resourceFactory->getImage("Box_Diffuse");

	MaterialInfo wallMaterial;
	wallMaterial.reset();
	wallMaterial.ambient = glm::vec3(0.05);
	wallMaterial.specular = glm::vec3(1);
	wallMaterial.shininess = 1;
	wallMaterial.diffuseTexture = engine->resourceFactory->getImage("Brick_Diffuse");

	//Generate a material for the crate using the shader and material info created
	pMaterial* tmpChairMatRef = engine->resourceFactory->createMaterial("Chair_Material", tmpPhongShaderRef, chairMaterial);
	pMaterial* tmpLightMatRef = engine->resourceFactory->createMaterial("LightSrc_Material", engine->resourceFactory->createDebugShader(), lightModelMaterial);
	pMaterial* tmpFloorMatRef = engine->resourceFactory->createMaterial("Floor_Material", tmpPhongShaderRef, floorMaterial);
	pMaterial* tmpWallMatRef = engine->resourceFactory->createMaterial("Wall_Material", tmpPhongShaderRef, wallMaterial);

	sphereRef = engine->resourceFactory->loadModel("Sphere_Model", "../Resources/Models/bunny.obj", tmpWallMatRef);
	sphereRef->setScale(glm::vec3(30.0f));
	
	//Create a box and set its material to look like a wooden crate
	chairRef = engine->resourceFactory->loadModel("Chair_Model", "../Resources/Models/chair/chair.obj", tmpChairMatRef);
	//chairRef2 = engine->resourceFactory->loadModel("Chair_Model2", "../Resources/Models/chair/chair.obj", tmpChairMatRef);

	//Create the light models (for visualisation of their locations)
	mdl_LightA = engine->resourceFactory->loadModel("LightSrc_A_Model", "../Resources/Models/uvsphere.obj", tmpLightMatRef);

	floorRef = engine->resourceFactory->createPrimitiveShape("Floor_Model", pPrimitiveMaker::CUBOID_TRI, glm::vec3(12, 0.25f, 12), glm::vec3(1), tmpFloorMatRef);
	wallRef = engine->resourceFactory->createPrimitiveShape("Wall_Model", pPrimitiveMaker::CUBOID_TRI, glm::vec3(12, 12, 0.25f), glm::vec3(1), tmpWallMatRef);

	//Scale the wall texture coordinates for a smaller repeated texture
	wallRef->scaleTextureCoordinates(glm::vec2(3));

	//Position and angle the camera view (this should be done in its own class eventually)
	engine->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 18.0f), glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//This overload of setProjectionMatrix() takes values for FOV
	engine->renderer->setProjectionMatrix(80, 1200, 1200, 0.2f, 240);

	//Move the models around to set up the scene initially
	//chairRef->setPosition(glm::vec3(-6.0f, -6.0f, -8.0f));
	//chairRef->setRotation(glm::vec3(0, 1, 0), 45);
	//chairRef->setScale(glm::vec3(0.5f));

	//Create 1 light in the scene						Type						Position				Range	Color		Intensity		Ambient		Attenuation
	light_A = engine->lightManager->addLight(new pLight(Light::LightType::POINT, glm::vec3(0.0f, 6.0f, 0.0), 24, glm::vec3(1), glm::vec3(1), glm::vec3(0.0f), 1.0f));

	//Set the light model positions
	mdl_LightA->setPosition(light_A->getPosition());
	mdl_LightA->getMaterial()->setDiffuseColor(light_A->getColor(), false);

	//Scale the light models
	mdl_LightA->setScale(glm::vec3(0.05f));


	lastMouseX = 0;
	lastMouseY = 0;

	scene = new pScene();

	pSceneObject* tmp = scene->createNewObject(chairRef, nullptr, nullptr, nullptr, glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(0.5), std::string("Bunny"), nullptr);


	//						Model	 Script	  SoundSystem  camera				position		rotation				scale					name			  parent
	tmp = scene->createNewObject(chairRef, nullptr, nullptr, nullptr, glm::vec3(4, 0, 0), glm::vec3(0), glm::vec3(1.0), std::string("chairObject"), tmp->getSceneNode());
	
	int offset = 4;
	for (int i = 0; i < 12; ++i) {
		tmp = scene->createNewObject(chairRef, nullptr, nullptr, nullptr, glm::vec3(offset, 0, 0), glm::vec3(offset * offset * 0.1, offset * offset * 0.6, offset * offset * 0.2), glm::vec3(1.2), std::string("chairObject"), tmp->getSceneNode());
		offset++;
	}


	for (int i = 0; i < scene->sceneGraph->getRootSceneNode()->attachedSceneNodeList.size(); ++i)
	{
		//std::cout << scene->sceneGraph->getRootSceneNode()->attachedSceneNodeList[i]->getName() << std::endl;
		//printf("Objects #: %i", i);
	}

	//std::cout << "Finding name of chair object...: " << scene->sceneGraph->findSceneNode(std::string("chairObject"), scene->sceneGraph->getRootSceneNode())->getName() << std::endl;
}

void TestScene::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TestScene::onRender()
{
	engine->renderer->renderModel(mdl_LightA);

	//Render geometry and chair
	//engine->renderer->renderModel(chairRef);
	
	//render the scene
	scene->sceneGraph->renderSceneGraph(engine->renderer, scene->sceneGraph->getRootSceneNode());

	//engine->renderer->renderModel(sphereRef);

	//floor
	floorRef->setPosition(glm::vec3(0, -12, 0));
	floorRef->setRotation(glm::vec3(1, 0, 0), 0);
	engine->renderer->renderModel(floorRef);

	//ceiling
	floorRef->setPosition(glm::vec3(0, 12, 0));
	floorRef->setRotation(glm::vec3(1, 0, 0), 0);
	engine->renderer->renderModel(floorRef);

	//back
	wallRef->setPosition(glm::vec3(0, 0, -12));
	wallRef->setRotation(glm::vec3(0, 1, 0), 0);
	engine->renderer->renderModel(wallRef);

	//left
	wallRef->setPosition(glm::vec3(-12, 0, 0));
	wallRef->setRotation(glm::vec3(0, 1, 0), 90);
	engine->renderer->renderModel(wallRef);

	//right
	wallRef->setPosition(glm::vec3(12, 0, 0));
	wallRef->setRotation(glm::vec3(0, 1, 0), 90);
	engine->renderer->renderModel(wallRef);
}

void TestScene::onPostRender()
{
	glfwSwapBuffers(engine->window);
}

void TestScene::onUpdate(GLdouble deltaTime)
{
	//printf("deltaTime is: ", std::to_string(deltaTime));

	pSceneNode* bunn = scene->sceneGraph->findSceneNode(std::string("Bunny"));
	pSceneNode* tmpNode = scene->sceneGraph->findSceneNode(std::string("chairObject"));

	glm::quat lookAtBunny = tmpNode->quaternionSlerp(tmpNode->getRotationQuat(), tmpNode->getLookAtPoint(bunn->getPosition()), deltaTime);



	//bunn->scaling(glm::vec3(1.001, 1.001, 1.001));

	//tmpNode->setRotation(lookAtBunny);

	//Light models follow light sources
	mdl_LightA->setPosition(light_A->getPosition());

	const float rotSpeed = 0.5;
	const float movSpeed = 0.6;

	if (glfwGetKey(engine->window, GLFW_KEY_R)) {
		bunn->rotateAround(glm::vec3(0, 1, 0), deltaTime * rotSpeed);
	}

	if (glfwGetKey(engine->window, GLFW_KEY_T)) {
		bunn->rotateAround(glm::vec3(0, 1, 0), deltaTime * -rotSpeed);
	}

	//Bunny movement
	if (glfwGetKey(engine->window, GLFW_KEY_W)) {
		bunn->translate(glm::vec3(0, movSpeed * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_S)) {
		bunn->translate(glm::vec3(0, -movSpeed * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_A)) {
		bunn->translate(glm::vec3(-movSpeed * deltaTime, 0, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_D)) {
		bunn->translate(glm::vec3(movSpeed * deltaTime, 0, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_Q)) {
		bunn->translate(glm::vec3(0.0f, 0.0f, movSpeed * deltaTime));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_E)) {
		bunn->translate(glm::vec3(0.0f, 0.0f, -movSpeed * deltaTime));
	}

	double lastX = lastMouseX;
	double lastY = lastMouseY;
	glfwGetCursorPos(engine->window, &lastMouseX, &lastMouseY);

	double diffX = lastMouseX - lastX;
	double diffY = lastMouseY - lastY;

	if (glfwGetMouseButton(engine->window, GLFW_MOUSE_BUTTON_1)) {
		bunn->rotateAround(glm::vec3(0, 1, 0), diffX * rotSpeed * deltaTime);
		bunn->rotateAround(glm::vec3(1, 0, 0), diffY * rotSpeed * deltaTime);
	}


	glfwPollEvents();
}

void TestScene::onPostUpdate()
{
}

void TestScene::onEnd()
{
	std::cout << "onEnd()" << std::endl;

	delete mdl_LightA;

	delete light_A;
}
