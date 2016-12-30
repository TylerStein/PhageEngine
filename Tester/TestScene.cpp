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
	//Brackets so this section can be collapsed ;)
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
	pShader* tmpPhongShaderRef = engine->resourceFactory->createShader("Phong_Lighting", atrMap, uniMap, "../Resources/Shaders/phong.vert", "../Resources/Shaders/phong.frag");

	//Recycle the maps for a new shader
	uniMap = uniformNameMap();
	atrMap = attribNameMap();
	{
		atrMap.insert(Attributes::VertexPosition, "vPosition");
		atrMap.insert(Attributes::VertexCoordinate, "vTexCoord");
		atrMap.insert(Attributes::VertexNormal, "vNormal");
		atrMap.insert(Attributes::VertexColor, "vColor");

		uniMap.insert(Uniforms::Camera_View, "cameraView");
		uniMap.insert(Uniforms::Projection_View, "projectionView");
		uniMap.insert(Uniforms::Model_View, "modelView");

		uniMap.insert(Uniforms::DiffuseColor, "diffuseColor");
	}
	pShader* tmpPrimShaderRef = engine->resourceFactory->createShader("Primitive_Shader", atrMap, uniMap, "../Resources/Shaders/primitive.vert", "../Resources/Shaders/primitive.frag");


	//Generate an image for the crate diffuse and bump maps
	engine->resourceFactory->createImage("Crate_Diffuse", "../Resources/Images/Crate/crate1_diffuse.png");
	engine->resourceFactory->createImage("Crate_Bump", "../Resources/Images/Crate/crate1_bump.png");

	engine->resourceFactory->createImage("Floor_Diffuse", "../Resources/Images/Floor/diffuse.tga");
	//engine->resourceFactory->createImage("Floor_bump", "../Resources/Images/bricks.png");

	//Manually create some material info for the crate and floor
	MaterialInfo crateMaterial;
	crateMaterial.reset();
	crateMaterial.diffuse = glm::vec3(1);
	crateMaterial.ambient = glm::vec3(0.15f);
	crateMaterial.specular = glm::vec3(0.1f);
	crateMaterial.shininess = 0.7f;
	crateMaterial.diffuseTexture = engine->resourceFactory->getImage("Crate_Diffuse");
	crateMaterial.useLight = true;

	MaterialInfo floorMaterial;
	floorMaterial.reset();
	floorMaterial.diffuse = glm::vec3(1);
	floorMaterial.ambient = glm::vec3(0.15f);
	floorMaterial.specular = glm::vec3(0.1f);
	floorMaterial.shininess = 0.7f;
	floorMaterial.diffuseTexture = engine->resourceFactory->getImage("Floor_Diffuse");

	MaterialInfo lightModelMaterial;
	lightModelMaterial.reset();
	lightModelMaterial.useLight = false;
	lightModelMaterial.diffuse = glm::vec3(1.0, 0.0, 0.0);

	//Generate a material for the crate using the shader and material info created
	pMaterial* tmpCrateMatRef = engine->resourceFactory->createMaterial("Crate_Material", tmpPhongShaderRef, crateMaterial);
	pMaterial* tmpFloorMatRef = engine->resourceFactory->createMaterial("Floor_Material", tmpPhongShaderRef, floorMaterial);
	pMaterial* tmpLightMatRef = engine->resourceFactory->createMaterial("LightSrc_Material", tmpPrimShaderRef, lightModelMaterial);

	//Generate the crate and floor models
	crateRef = engine->resourceFactory->createModel("Crate_Model", tmpCrateMatRef, cubeVerts, cubeNormals, cubeColors, cubeTexCoords, cubeVertCount, GL_TRIANGLES);
	floorRef = engine->resourceFactory->createModel("Floor_Model", tmpFloorMatRef, planeVerts, planeNormals, planeColors, planeUVs, planeVertCount, GL_TRIANGLES);
	lightSourceRef = engine->resourceFactory->createModel("LightSrc_Model", tmpLightMatRef, cubeVerts, cubeNormals, cubeColors, cubeTexCoords, cubeVertCount, GL_TRIANGLES);

	//Position and angle the camera view (this should be done in its own class eventually)
	engine->renderer->setViewMatrix(glm::vec3(0.0f, 5.0f, 13.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//This overload of setProjectionMatrix() takes values for FOV
	engine->renderer->setProjectionMatrix(80, 1200, 1200, 0.2, 120);

	//Move the models around to set up the scene initially
	crateRef->setPosition(glm::vec3(0.0f, 0.0f, -2.0f));
	crateRef->rotateAround(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f);

	floorRef->scale(glm::vec3(2.0f, 2.0f, 2.0f));
	floorRef->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	floorRef->setRotation(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	floorRef->setScale(glm::vec3(8.0f, 8.0f, 1.0f));

	lightSourceRef->setScale(glm::vec3(0.2f));



	//Create 2 colored lights in the scene
	lightARef = engine->lightManager->addLight(new pLight(glm::vec3(-4.0f, 2.0f, 0.0f), 25.0f, glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.25f), glm::vec3(0.05f), 1.0f));
	lightBRef = engine->lightManager->addLight(new pLight(glm::vec3(4.0f, 2.0f, 0.0f), 25.0f, glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.25f), glm::vec3(0.05f), 1.0f));
}

void TestScene::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TestScene::onRender()
{
	//Render red light cube
	lightSourceRef->setPosition(lightARef->getPosition());
	lightSourceRef->getMaterial()->setDiffuseColor(lightARef->getColor(), false);

	engine->renderer->renderModel(lightSourceRef);
	//Render blue light cube
	lightSourceRef->setPosition(lightBRef->getPosition());
	lightSourceRef->getMaterial()->setDiffuseColor(lightBRef->getColor(), false);
	engine->renderer->renderModel(lightSourceRef);

	//Render floor and crate
	engine->renderer->renderModel(floorRef);
	engine->renderer->renderModel(crateRef);
}

void TestScene::onPostRender()
{
	glfwSwapBuffers(engine->window);
}

void TestScene::onUpdate(GLdouble deltaTime)
{
	if (glfwGetKey(engine->window, GLFW_KEY_R)) {
		floorRef->getMaterial()->getShader()->compileShader();
	}

	if (glfwGetKey(engine->window, GLFW_KEY_T)) {
		lightSourceRef->getMaterial()->getShader()->compileShader();
	}

	//Cube movement
	if (glfwGetKey(engine->window, GLFW_KEY_W)) {
		crateRef->translate(glm::vec3(0, 1 * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_S)) {
		crateRef->translate(glm::vec3(0, -1 * deltaTime, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_A)) {
		crateRef->translate(glm::vec3(-1 * deltaTime, 0, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_D)) {
		crateRef->translate(glm::vec3(1 * deltaTime, 0, 0));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_Q)) {
		crateRef->translate(glm::vec3(0.0f, 0.0f, 1.0f * deltaTime));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_E)) {
		crateRef->translate(glm::vec3(0.0f, 0.0f, -1.0f * deltaTime));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_UP)) {
		lightARef->setPosition(lightARef->getPosition() + glm::vec3(0.0f, 0.0f, -1.0f * deltaTime));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_DOWN)) {
		lightARef->setPosition(lightARef->getPosition() + glm::vec3(0.0f, 0.0f, 1.0f * deltaTime));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_LEFT)) {
		lightARef->setPosition(lightARef->getPosition() + glm::vec3(-1.0f * deltaTime, 0.0f, 0.0f));
	}

	if (glfwGetKey(engine->window, GLFW_KEY_RIGHT)) {
		lightARef->setPosition(lightARef->getPosition() + glm::vec3(1.0f * deltaTime, 0.0f, 0.0f));
	}

	//Cube rotation
	crateRef->rotateAround(glm::vec3(0, 1, 1), 5 * deltaTime);


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
}
