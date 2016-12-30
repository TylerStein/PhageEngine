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

	//Use the shader program
	glUseProgram(model->getShaderProgramID());

	//Apply the model's mateiral
	model->UseMaterial();

	//Apply the model position matrix
	glUniformMatrix4fv(model->getModelMatrixID(), 1, GL_FALSE, &model->getModelMatrix()[0][0]);
	//Apply the view matrix
	glUniformMatrix4fv(model->getViewMatrixID(), 1, GL_FALSE, &cameraView[0][0]);
	//Apply the perspective matrix
	glUniformMatrix4fv(model->getProjectionMatrixID(), 1, GL_FALSE, &projMatrix[0][0]);

	//Draw the model's points from the currently bound VAO with currently used shader
	glDrawArrays(model->getDrawMode(), 0, model->getVertCount());

	model->getMaterial()->unuseMaterial();

	glUseProgram(0);
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

void pRenderer::setProjectionMatrix(GLfloat fov, GLfloat windowWidth, GLfloat windowHeight, GLfloat near, GLfloat far)
{
	float w = windowWidth;
	float h = windowHeight;
	float ratio = w / h;
	float top = tan(fov * glm::pi<float>() / 360.0f) * near;
	float bottom = -top;
	float left = ratio * bottom;
	float right = ratio * top;
	setProjectionMatrix(left, right, bottom, top, near, far);
}

void pRenderer::initDefaultView()
{
	setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 10.0f);
}
