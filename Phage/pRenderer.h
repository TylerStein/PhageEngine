#pragma once
#include "pModel.h"
#include "pLight.h"
#include "GLFW\glfw3.h"
#include "glm\vec3.hpp"
#include "glm\gtc\matrix_transform.hpp"

class pRenderer
{
public:
	pRenderer(GLFWwindow* window);
	~pRenderer();

	void setupGLOptions();
	void setWindowRef(GLFWwindow* window);
	void renderModel(pModel* model);

	void setViewMatrix(glm::vec3 cameraLocation, glm::vec3 cameraTarget, glm::vec3 cameraUp);
	void setProjectionMatrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearPlane, GLfloat farPlane);
	void setProjectionMatrix(GLfloat fov, GLfloat windowWidth, GLfloat windowHeight, GLfloat near, GLfloat far);
private:
	void initDefaultView();

	GLFWwindow* windowRef;
	
	GLuint vertexBuffer;

	glm::vec3 camLoc, camTar, camUp;
	glm::mat4 projMatrix;
	glm::mat4 cameraView;

	GLuint texture[1];
};

