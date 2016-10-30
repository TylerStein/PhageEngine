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

	//Store the names of variables the loaded shader takes
	ShaderInfo simpleShaderInfo;
	simpleShaderInfo.modelViewAttribute = "model_matrix";
	simpleShaderInfo.projectionAttribute = "projection_matrix";
	simpleShaderInfo.viewAttribute = "view_matrix";
	simpleShaderInfo.vertexPositionAttribute = "vPosition";
	simpleShaderInfo.vertexColorAttribute = "vColor";
	simpleShaderInfo.vertexCoordinateAttribute = "vTexCoord";

	//Create the shader
	simpleShader = engine->resourceFactory->createShader("SimpleShader", "../Resources/Shaders/SimpleVertexShader.vert", "../Resources/Shaders/SimpleFragmentShader.frag", simpleShaderInfo);

	//Generate an image called TestImage
	testImg = engine->resourceFactory->createImage("TestImage", "../Resources/Images/bricks.png");

	//Manually create some material info
	MaterialInfo matInfo;
	matInfo.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	matInfo.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	matInfo.alpha = 1.0f;
	matInfo.shininess = 1.0f;
	matInfo.bHasTexture = true;
	matInfo.textureList = std::vector<pImage*>();
	matInfo.textureList.push_back(testImg);
	matInfo.blendMode = GL_ADD;

	//Generate a material called TestMat
	testMat = engine->resourceFactory->createMaterial("TestMat", simpleShader, matInfo);

	//Generate a model called TestModel
	testModel = engine->resourceFactory->createModel("TestModel", testMat, testVerts, testNormals, testColors, testUVs, testVertCount);

	//Set the renderer view matrix
	engine->renderer->setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	engine->renderer->setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 5.0f);

	std::cout << "Image Width: " << engine->resourceFactory->getImage("TestImage")->getWidth() << std::endl;
	std::cout << "Model's Material Name: " << engine->resourceFactory->getModel("TestModel")->getMaterial()->getName() << std::endl;

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

void TestScene::onUpdate(GLdouble deltaTime)
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
