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

	/*
	//Make a cube
	const GLuint testVertCount = 24;
	GLfloat testVerts[] = {
		//Front face
		-1, 1, 1,
		1, 1, 1,
		1, -1, 1,
		-1, -1, 1,

		//Left-Side face
		-1, -1, 1,
		-1, 1, 1,
		-1, 1, -1,
		-1, -1, -1,

		//Bottom Face
		-1, -1, -1,
		-1, -1, 1,
		1, -1, 1,
		1, -1, -1,

		//Right-Side face
		1, -1, -1,
		1, -1, 1,
		1, 1, 1,
		1, 1, -1,

		//Top face
		1, 1, -1,
		1, 1, 1,
		-1, 1, 1,
		-1, 1, -1,

		//Back face
		-1, 1, -1,
		-1, -1, -1,
		1, -1, -1,
		1, 1, -1
	};

	GLfloat testUVs[] = {
		0, 1,
		1, 1,
		1, 0,
		0, 0,

		1, 0,
		1, 1,
		0, 1,
		0, 0,

		0, 1,
		0, 0,
		1, 0,
		1, 1,

		1, 0,
		0, 0,
		0, 1,
		1, 1,

		1, 1,
		1, 0,
		0, 0,
		0, 1,

		1, 1,
		1, 0,
		0, 0,
		0, 1
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

	GLfloat testNormals[] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};*/

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

	//Create the shader
	simpleShader = engine->resourceFactory->createShader("SimpleShader", "../Resources/Shaders/SimpleVertexShader.vert", "../Resources/Shaders/SimpleFragmentShader.frag", simpleShaderInfo);

	//Generate an image called TestImage
	testImg = engine->resourceFactory->createImage("TestImage", "../Resources/Images/bricks.png");
	//testImg = engine->resourceFactory->createDebugImage("TestImage");

	//Manually create some material info
	MaterialInfo matInfo;
	matInfo.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	matInfo.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	matInfo.alpha = 1.0f;
	matInfo.shininess = 1.0f;
	matInfo.bHasTexture = true;
	matInfo.texture0 = testImg;
	matInfo.blendMode = GL_ADD;

	//Generate a material called TestMat
	testMat = engine->resourceFactory->createMaterial("TestMat", simpleShader, matInfo);

	//Generate a model called TestModel
	testModel = engine->resourceFactory->createModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount, GL_QUADS);

	//Set the renderer view matrix
	engine->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	engine->renderer->setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 5.0f);

	//Set the position of the model to 0, 0, 0
	testModel->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));

	//Set the rotation of the model Z to 0
	testModel->rotateAround(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);
	
	std::cout << "Image Info: \n";
	std::cout << "Width: " << engine->resourceFactory->getImage("TestImage")->getWidth() << std::endl;
	std::cout << "Height: " << engine->resourceFactory->getImage("TestImage")->getHeight() << std::endl;
	std::cout << "TextureID: " << engine->resourceFactory->getImage("TestImage")->getTextureID ()<< std::endl;

	LogManager::instance()->warning("Here be the log manager.");
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

void TestScene::onUpdate(GLdouble deltaTime)
{
	testModel->rotateAround(glm::vec3(0, 1, 0), 5 * deltaTime);
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
	delete loadedModel;
}
