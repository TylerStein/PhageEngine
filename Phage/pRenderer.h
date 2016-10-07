#pragma once
#include "pModel.h"
#include "GLFW\glfw3.h"


class pRenderer
{
public:
	pRenderer(GLFWwindow* window);
	~pRenderer();

	void initRender();
	void setupGLOptions();
	void setWindowRef(GLFWwindow* window);
	void renderModel(pModel* model);

private:
	GLFWwindow* windowRef;
	
	GLuint vertexBuffer;
};

