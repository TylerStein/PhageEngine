#pragma once
#include "pModel.h"
#include "pLight.h"
#include "GLFW\glfw3.h"
#include "glm\vec3.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "pCubeMap.h"
#include "pSceneNode.h"

class pRenderer
{
public:
	pRenderer(GLFWwindow* window);
	~pRenderer();

	void setupGLOptions();
	void setWindowRef(GLFWwindow* window);

	void renderSceneNode(pSceneNode* node, glm::mat4x4 mat);

	void renderModel(pModel* model);

	void setSkybox(pCubeMap* cubeMap);
	void renderSkybox();
	
	void setViewMatrix(glm::vec3 cameraLocation, glm::vec3 cameraTarget, glm::vec3 cameraUp);
	void setProjectionMatrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearPlane, GLfloat farPlane);
	void setProjectionMatrix(GLfloat fov, GLfloat windowWidth, GLfloat windowHeight, GLfloat near, GLfloat far);

	void setProjectionMatrix(glm::mat4 projMatrix);
	void setViewMatrix(glm::mat4 viewMatrix);

	inline glm::mat4 getProjectionMatrix()
	{
		return projMatrix;
	}

	inline glm::mat4 getViewMatrix()
	{
		return cameraView;
	}

private:
	void initDefaultView();

	GLFWwindow* windowRef;

	pCubeMap* skybox;

	glm::vec3 camLoc, camTar, camUp;
	glm::mat4 projMatrix;
	glm::mat4 cameraView;
};

