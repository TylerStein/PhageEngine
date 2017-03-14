#include "DebugEditor.h"
#include "GUIHelper.h"
#include "glm/gtc/matrix_transform.hpp"

#include "ExampleScript.h"

DebugEditor::DebugEditor()
{
}


DebugEditor::~DebugEditor()
{
}

void DebugEditor::giveEngineReference(PhageEngine * engine)
{
	this->engine = engine;
}

void DebugEditor::onStart()
{
	std::cout << "Editor Start()" << std::endl;

	//For frame draw time display
	lastTime = glfwGetTime();
	numFrames = 0;
	lastFrameTime = 0.0;

	//Set up ImGui
	ImGui_ImplGlfwGL3_Init(engine->window, true);

	//Create the shader attributes and uniforms to link with the shader being used
	uniformNameMap uniMap = uniformNameMap();
	attribNameMap atrMap = attribNameMap();
	//Brackets so this section can be collapsed
	{
		atrMap.insert(Attributes::VertexPosition, "vPosition");
		atrMap.insert(Attributes::VertexCoordinate, "vTexCoord");
		atrMap.insert(Attributes::VertexNormal, "vNormal");

		uniMap.insert(Uniforms::Projection_View, "projView");
		uniMap.insert(Uniforms::Model_View, "modelView");
		uniMap.insert(Uniforms::Camera_View, "cameraView");
		uniMap.insert(Uniforms::Normal_View, "normalView");

		uniMap.insert(Uniforms::Camera_Position, "cameraLocation");

		uniMap.insert(Uniforms::DiffuseColor, "fDiffuse");
		uniMap.insert(Uniforms::AmbientColor, "fAmbient");
		uniMap.insert(Uniforms::DiffuseTexture, "fTexture");
		uniMap.insert(Uniforms::SpecularTexture, "fSpecularMap");
		uniMap.insert(Uniforms::SpecularColor, "fSpecular");
		uniMap.insert(Uniforms::Shininess, "fShininess");

		uniMap.insert(Uniforms::Light_Count, "lightCount");
		uniMap.insert(Uniforms::Light_Type, "lType");
		uniMap.insert(Uniforms::Light_Range, "lRange");
		uniMap.insert(Uniforms::Light_Position, "lPosition");
		uniMap.insert(Uniforms::Light_Angle, "lAngle");
		uniMap.insert(Uniforms::Light_Cone, "lCone");
		uniMap.insert(Uniforms::Light_Color, "lColor");
		uniMap.insert(Uniforms::Light_Power, "lPower");
		uniMap.insert(Uniforms::Light_Ambient, "lAmbient");
	}
	//Create a shader for the shape surfaces
	engine->resourceFactory->createShader("Phong_Lighting", atrMap, uniMap, "../Resources/Shaders/PhongMaterial.vert", "../Resources/Shaders/PhongMaterial.frag");
	
	engine->resourceFactory->createImage("Metal_Diffuse", "../Resources/Images/Metal/diffuse.jpg");
	engine->resourceFactory->createImage("Metal_Specular", "../Resources/Images/Metal/specular.jpg");
	engine->resourceFactory->createImage("Metal_Normal", "../Resources/Images/Metal/normal.jpg");

	engine->resourceFactory->createImage("Oildrum_Diffuse", "../Resources/Models/Oildrum/oildrum_diffuse.jpg");
	engine->resourceFactory->createImage("Oildrum_Specular", "../Resources/Images/Metal/oildrum_specular.jpg");
	engine->resourceFactory->createImage("Oildrum_Normal;", "../Resources/Images/Metal/oildrum_normal.jpg");

	//==Create a material using the previously created shader and images==//
	pMaterial* tmpMatRef = engine->resourceFactory->createMaterial("Oildrum_Material", 
		engine->resourceFactory->getShader("Phong_Lighting"), 
		MaterialInfo{
			glm::vec3(1.0f),	//Diffuse
			glm::vec3(0.1f),	//Ambient	
			glm::vec3(1.4f),	//Specular
			glm::vec3(0),		//Emit
			16.0,				//Shininess
			engine->resourceFactory->getImage("Oildrum_Diffuse"),	//Diffuse tex
			engine->resourceFactory->getImage("Oildrum_Specular")	//Specular tex
	});
	tmpMatRef->setUseLight(true);
	
	//==Creating a directional (sun) light and adding it to the light manager==/
	pLight* tmpLightRef = engine->lightManager->addLight(new pLight(Light::DIRECTIONAL, glm::vec3(-0.8f, 0.8f, -0.3f), glm::vec3(1.0f, 0.96f, 0.8f), glm::vec3(1.0f, 0.96f, 0.8f), glm::vec3(0.08f)));

	//==Create a scene to work with and get the reference to it==//
	engine->sceneManager->addScene();
	pScene* scene = engine->sceneManager->getCurrentScene();

	//==Creating the Oildrum object and add it to the scene graph==//
	pSceneObject* drumObject = new pSceneObject(); //Create empty scene object

	pModel* tmpModelRef = engine->resourceFactory->loadModel("Model_Oildrum", "../Resources/Models/Oildrum/oildrum.obj", tmpMatRef); //Create temporary model ref
	pScript* tmpScript = engine->resourceFactory->addScript("Script_Oildrum", new ExampleScript(drumObject)); //Create temporary script reff

	//add the model and script to the Oildrum scene object
	drumObject->attachModel(tmpModelRef);
	drumObject->attachScript(tmpScript);

	//Add the bunny object to the scene graph
	//scene->addExistingObject(drumObject, "Oildrum_Object", glm::vec3(0.0f), glm::vec3(0, 90, 0), 5.0f, glm::vec3(2.0f));

	//==Creating a skybox and adding it to the renderer==/
	pCubeMap* skyboxCube = new pCubeMap("../Resources/Cubemaps/Citadella/posx.jpg",
		"../Resources/Cubemaps/Citadella/negx.jpg",
		"../Resources/Cubemaps/Citadella/posy.jpg",
		"../Resources/Cubemaps/Citadella/negy.jpg",
		"../Resources/Cubemaps/Citadella/posz.jpg",
		"../Resources/Cubemaps/Citadella/negz.jpg"
	);
	engine->renderer->setSkybox(skyboxCube);

	//==Creating a camera and attaching it to a scene object==//
	pCamera* tmpCam = new pCamera(engine->windowWidth, engine->windowHeight, 60.0f, 120.0f, 0.2f);
	cameraObject = scene->createNewObject(nullptr, nullptr, nullptr, tmpCam, glm::vec3(0, 0, -12), glm::vec3(0, 0, 1), glm::vec3(1), "Camera_Object");

	
	//Send camera projection matrix to the renderer
	engine->renderer->setProjectionMatrix(cameraObject->getAttachedCamera()->getProjectionMatrix());

	//Manually make a view matrix
	viewMat = glm::lookAt(glm::vec3(0, 0, -12), glm::vec3(0), glm::vec3(0, 1, 0));
	engine->renderer->setViewMatrix(viewMat);
}

void DebugEditor::onUpdate(GLdouble deltaTime)
{

	double oldX = lastMouseX;
	double oldY = lastMouseY;
	glfwGetCursorPos(engine->window, &lastMouseX, &lastMouseY);

	if (glfwGetKey(engine->window, GLFW_KEY_LEFT)) {
		viewMat = glm::rotate(viewMat, (float)(deltaTime * 0.5f), glm::vec3(0, 1, 0));
	}
	if (glfwGetKey(engine->window, GLFW_KEY_RIGHT)) {
		viewMat = glm::rotate(viewMat, (float)(deltaTime * -0.5f), glm::vec3(0, 1, 0));
	}

	engine->renderer->setViewMatrix(viewMat);

	float changeX = lastMouseX - oldX;
	float changeY = lastMouseY - oldY;

}

void DebugEditor::onPostUpdate()
{

}

void DebugEditor::onPreRender()
{
	//Clear the drawing surface
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Create new ImGui frame
	ImGui_ImplGlfwGL3_NewFrame();
}

void DebugEditor::onRender()
{
	ImGui::Begin("Debug");
	{
		ImGui::Text("Render Time (ms): %f", lastFrameTime);
	}
	ImGui::End();

	//skybox
	engine->renderer->renderSkybox();
}

void DebugEditor::onPostRender()
{
	//Render the ImGui GUI
	ImGui::Render();

	glfwSwapBuffers(engine->window);

	//End of loop time count
	double currentTime = glfwGetTime();
	numFrames++;

	double diff = (currentTime - lastTime);
	if (diff >= 1.0) {
		//printf("%f ms/frame\n", 1000.0 / double(numFrames));
		lastFrameTime = (float)(1000.0 / double(numFrames));
		numFrames = 0;
		lastTime += diff;
	}
}

void DebugEditor::onEnd()
{
	std::cout << "Editor End()" << std::endl;

	//Shut down and clean up ImGui
	ImGui_ImplGlfwGL3_Shutdown();
}
