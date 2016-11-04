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

	//Make a plane for the floor
	GLfloat floorVertCount = 4;
	GLfloat	floorVerts[] = {
		-1, -1, 0,
		1, -1, 0,
		1, 1, 0,
		-1, 1, 0
	};

	GLfloat floorUVs[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	GLfloat floorColors[] = {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		1, 1, 1
	};

	GLfloat floorNormals[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	GLfloat testVertCount = 24;
	GLfloat testVerts[] = {
		//front cw
		-1, -1, 1,
		-1, 1, 1,
		1, 1, 1,
		1, -1, 1,

		//bottom cw
		1, -1, 1,
		-1, -1, 1,
		-1, -1, -1,
		1, -1, -1,

		//back cw
		1, -1, -1,
		1, 1, -1,
		-1, 1, -1,
		-1, -1, -1,

		//left ccw
		-1, -1, -1,
		-1, -1, 1,
		-1, 1, 1,
		-1, 1, -1,

		//top cw
		-1, 1, -1,
		-1, 1, 1,
		1, 1, 1,
		1, 1, -1,

		//right ccw
		1, 1, -1,
		1, 1, 1,
		1, -1, 1,
		1, -1, -1
	};

	GLfloat testColors[] = {
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

	GLfloat testUVs[] = {
		//front cw
		0, 0,
		0, 1,
		1, 1,
		1, 0,

		//bottom cw
		0, 0,
		0, 1,
		1, 1,
		1, 0,

		//back cw
		0, 0,
		0, 1,
		1, 1,
		1, 0,

		//left ccw
		0, 0,
		1, 0,
		1, 1,
		0, 1,

		//top cw/broken
		0, 1,
		0, 0,
		1, 0,
		1, 1,

		//right
		1, 1,
		0, 1,
		0, 0,
		1, 0

	};

	GLfloat testNormals[] = {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,

		0, -1, 0,
		0, -1, 0,
		0, -1, 0,
		0, -1, 0,

		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,

		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,

		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0
	};

	//Store the names of variables the loaded shader takes
	ShaderInfo simpleShaderInfo;
	simpleShaderInfo.modelViewAttribute = "model_matrix";
	simpleShaderInfo.projectionAttribute = "projection_matrix";
	simpleShaderInfo.viewAttribute = "view_matrix";
	simpleShaderInfo.vertexPositionAttribute = "vPosition";
	simpleShaderInfo.vertexColorAttribute = "vColor";
	simpleShaderInfo.vertexCoordinateAttribute = "vTexCoord";
	simpleShaderInfo.vertexNormalAttribute = "vNormal";
	simpleShaderInfo.ambientAttribute = "ambient";
	simpleShaderInfo.diffuseAttribute = "diffuse";
	simpleShaderInfo.specularAttribute = "specular";
	simpleShaderInfo.shininessAttribute = "shininess";
	simpleShaderInfo.alphaAttribute = "alpha";
	simpleShaderInfo.lightPosAttribute = "lightsource";
	simpleShaderInfo.lightPowAttribute = "lightpower";

	//Create the shader
	simpleShader = engine->resourceFactory->createShader("SimpleShader", "../Resources/Shaders/SimpleVertexShader.vert", "../Resources/Shaders/SimpleFragmentShader.frag", simpleShaderInfo);

	//Generate an image called TestImage
	testImg = engine->resourceFactory->createImage("TestImage", "../Resources/Images/bricks.png");

	//Manually create some material info
	MaterialInfo matInfo;
	matInfo.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	matInfo.ambient = glm::vec3(0.4f, 0.4f, 0.4f);
	matInfo.specular = glm::vec3(0.2f, 0.2f, 0.2f);
	matInfo.alpha = 1.0f;
	matInfo.shininess = 0.8f;
	matInfo.bHasTexture = true;
	matInfo.texture0 = testImg;
	matInfo.blendMode = GL_ADD;

	//Generate a material called TestMat
	testMat = engine->resourceFactory->createMaterial("TestMat", simpleShader, matInfo);

	//Generate a model called TestModel
	testModel = engine->resourceFactory->createModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount, GL_QUADS);
	floor = engine->resourceFactory->createModel("Floor", testMat, floorVerts, floorNormals, floorColors, floorUVs, floorVertCount, GL_QUADS);

	//Set the renderer view matrix
	engine->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	engine->renderer->setProjectionMatrix(-0.25f, 0.25f, -0.25f, 0.25f, 0.2f, 50.0f);

	//Set the position of the model to 0, 0, 0
	testModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	floor->setPosition(glm::vec3(0.0f, 0.0, -1.0f));

	//Set the rotation of the model Z to 0
	testModel->rotateAround(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	floor->setRotation(glm::vec3(0.0f, 0.0f, 1.0f), 0.0);

	floor->setScale(glm::vec3(8.0f, 8.0f, 1.0f));

	engine->renderer->sceneLight = new pLight(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(1, 1, 1));
}

void TestScene::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TestScene::onRender()
{
	engine->renderer->renderModel(floor);
	engine->renderer->renderModel(testModel);
}

void TestScene::onPostRender()
{
	glfwSwapBuffers(engine->window);
}

void TestScene::onUpdate(GLdouble deltaTime)
{
	//Light movement
	if (glfwGetKey(engine->window, GLFW_KEY_I)) {
		engine->renderer->sceneLight->translate(glm::vec3(0.0f, 1.0f * deltaTime, 0.0f));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_K)) {
		engine->renderer->sceneLight->translate(glm::vec3(0.0f, -1.0f * deltaTime, 0.0f));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_J)) {
		engine->renderer->sceneLight->translate(glm::vec3(-1.0f * deltaTime, 0.0f, 0.0f));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_L)) {
		engine->renderer->sceneLight->translate(glm::vec3(1.0f * deltaTime, 0.0f, 0.0f));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_U)) {
		engine->renderer->sceneLight->translate(glm::vec3(0.0f, 0.0f, 1.0f * deltaTime));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_O)) {
		engine->renderer->sceneLight->translate(glm::vec3(0.0f, 0.0f, -1.0f * deltaTime));
	}

	//Cube movement
	if (glfwGetKey(engine->window, GLFW_KEY_W)) {
		testModel->translate(glm::vec3(0, 1 * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_S)) {
		testModel->translate(glm::vec3(0, -1 * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_A)) {
		testModel->translate(glm::vec3(-1 * deltaTime, 0, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_D)) {
		testModel->translate(glm::vec3(1 * deltaTime, 0, 0));
	}

	//Cube rotation
	testModel->rotateAround(glm::vec3(0, 1, 1), 5 * deltaTime);


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
