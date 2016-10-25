#pragma once
#include <vector>
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "pRenderer.h"
#include "pResourceFactory.h"

/*Class within the library to act as a palceholder in early development*/

class PhageEngine
{
public:
	PhageEngine();
	~PhageEngine();

	//Placeholder window creation method
	void CreateWindow(GLint width, GLint height, char* title);
	void Start();

	void onPreRender();
	void onRender();
	void onPostRender();
	void onUpdate();
	void onPostUpdate();

	pRenderer* renderer;
	pResourceFactory* resourceFactory;

	std::vector<pModel*> modelList;



private:
	void doLoop();
	GLFWwindow* window;

	pModelManager* modelManager;
	pMaterialManager* materialManager;
	pImageManager* imageManager;
};

