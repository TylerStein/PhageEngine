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

	glEnable(GL_PROGRAM_POINT_SIZE_EXT);
	glPointSize(5);
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

	pShader* shdr = model->getMaterial()->getShader();

	//Apply the model position matrix
	if (shdr->hasUniform(Uniforms::Model_View)) {
		glUniformMatrix4fv(model->getModelMatrixID(), 1, GL_FALSE, &model->getModelMatrix()[0][0]);
	}
	//Apply the view matrix
	if (shdr->hasUniform(Uniforms::Camera_View)) {
		glUniformMatrix4fv(model->getViewMatrixID(), 1, GL_FALSE, &cameraView[0][0]);
	}
	//Apply the perspective matrix
	if (shdr->hasUniform(Uniforms::Projection_View)) {
		glUniformMatrix4fv(model->getProjectionMatrixID(), 1, GL_FALSE, &projMatrix[0][0]);
	}
	//Apply the normal matrix
	if (shdr->hasUniform(Uniforms::Normal_View)) {
		glUniformMatrix3fv(model->getNormalMatrixID(), 1, GL_FALSE, &model->getNormalMatrix()[0][0]);
	}

	//Feed the model the camera position
	if (shdr->hasUniform(Uniforms::Camera_Position)) {
		glUniform3f(shdr->getUniformID(Uniforms::Camera_Position), camLoc.x, camLoc.y, camLoc.z);
	}

	if (model->usesIndeces()) {
		//Bind the buffer of the model's indexed vertices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getElementBufferID());
		//Draw the model's points from the currently bound EBO with currently used shader
		glDrawElements(model->getDrawMode(), model->getNumIndeces(), GL_UNSIGNED_INT, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else {
		//Draw the model's points from the currently bound VAO with currently used shader
		glDrawArrays(model->getDrawMode(), 0, model->getVertCount());
	}

	model->getMaterial()->unuseMaterial();

	glBindVertexArray(0);
	glUseProgram(0);
}

void pRenderer::renderSceneNode(pSceneNode * node, glm::mat4x4 mat)
{
	pModel* model = node->getAttachedSceneObject()->getAttachedModel();

	if (model != nullptr)
	{
		//Bind the model's vertex array id
		glBindVertexArray(model->getVertexArrayID());//Bind the model's vertex array id

		//Use the shader program
		glUseProgram(model->getShaderProgramID());

		//Apply the model's mateiral
		model->UseMaterial();

		pShader* shdr = model->getMaterial()->getShader();

		//Apply the model position matrix
		if (shdr->hasUniform(Uniforms::Model_View)) {
			glUniformMatrix4fv(model->getModelMatrixID(), 1, GL_FALSE, &mat[0][0]);
		}

		//Apply the view matrix
		if (shdr->hasUniform(Uniforms::Camera_View)) {
			glUniformMatrix4fv(model->getViewMatrixID(), 1, GL_FALSE, &cameraView[0][0]);
		}
		//Apply the perspective matrix
		if (shdr->hasUniform(Uniforms::Projection_View)) {
			glUniformMatrix4fv(model->getProjectionMatrixID(), 1, GL_FALSE, &projMatrix[0][0]);
		}
		//Apply the normal matrix
		if (shdr->hasUniform(Uniforms::Normal_View)) {
			glUniformMatrix3fv(model->getNormalMatrixID(), 1, GL_FALSE, &model->getNormalMatrix()[0][0]);
		}

		//Feed the model the camera position
		if (shdr->hasUniform(Uniforms::Camera_Position)) {
			glUniform3f(shdr->getUniformID(Uniforms::Camera_Position), camLoc.x, camLoc.y, camLoc.z);
		}

		if (model->usesIndeces()) {
			//Bind the buffer of the model's indexed vertices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->getElementBufferID());
			//Draw the model's points from the currently bound EBO with currently used shader
			glDrawElements(model->getDrawMode(), model->getNumIndeces(), GL_UNSIGNED_INT, nullptr);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		else {
			//Draw the model's points from the currently bound VAO with currently used shader
			glDrawArrays(model->getDrawMode(), 0, model->getVertCount());
		}

		model->getMaterial()->unuseMaterial();
	}
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
