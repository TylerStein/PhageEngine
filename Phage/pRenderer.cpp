#include "pRenderer.h"
#include <iostream>
#include "GLError.h"

const GLfloat bg[] = { 0.0f, 0.0f, 0.0f, 0.0f };

pRenderer::pRenderer(GLFWwindow* window)
{
	setWindowRef(window);
	setupGLOptions();
	initDefaultView();
}


pRenderer::~pRenderer()
{

}

void pRenderer::setupGLOptions() {
	glEnable(GL_DEPTH_TEST); //Enable depth-testing
	glDepthFunc(GL_LESS); //Interpret smaller values as closer
}

void pRenderer::setWindowRef(GLFWwindow* window)
{
	windowRef = window;
}

void pRenderer::renderModel(pModel* model)
{
	//Bind the model's vertex array id
	glBindVertexArray(model->getVertexArrayID());

	//Link the shader program ID
	glLinkProgram(model->getShaderProgramID());
	//Use the shader program!
	glUseProgram(model->getShaderProgramID());

	//Apply the model position matrix
	glUniformMatrix4fv(model->getModelMatrixID(), 1, GL_FALSE, &model->getModelMatrix()[0][0]);
	//Apply the view matrix
	glUniformMatrix4fv(model->getViewMatrixID(), 1, GL_FALSE, &cameraView[0][0]);
	//Apply the perspective matrix
	glUniformMatrix4fv(model->getProjectionMatrixID(), 1, GL_FALSE, &projMatrix[0][0]);


	//Send the scene light to the material
	model->getMaterial()->setLightEffect(sceneLight->getLight());

	//Draw the model's points from the currently bound VAO with currently used shader
	glDrawArrays(model->getDrawMode(), 0, model->getVertCount());
}

void pRenderer::setViewMatrix(glm::vec3 cameraLocation, glm::vec3 cameraTarget, glm::vec3 cameraUp)
{
	camLoc = cameraLocation;
	camTar = cameraTarget;
	camUp = cameraUp;
	cameraView = glm::lookAt(camLoc, camTar, camUp);
}

void pRenderer::setProjectionMatrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearPlane, GLfloat farPlane)
{
	projMatrix = glm::frustum(left, right, bottom, top, nearPlane, farPlane);
}

void pRenderer::initDefaultView()
{
	setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 10.0f);
}
