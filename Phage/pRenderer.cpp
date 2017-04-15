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

void pRenderer::renderSceneNode(pSceneNode * node, glm::mat4x4 mat)
{
	pModel* model = node->getAttachedSceneObject()->getAttachedModel();

	if (model != nullptr)
	{
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
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(mat));
			glUniformMatrix3fv(model->getNormalMatrixID(), 1, GL_FALSE, &normalMatrix[0][0]);
		}

		if (shdr->hasUniform(Uniforms::ModelViewProjection)) {
			glm::mat4 MVP = projMatrix * cameraView * mat;
			glUniformMatrix4fv(model->getMVPMatrixID(), 1, GL_FALSE, &MVP[0][0]);
		}

		//Feed the model the camera position
		if (shdr->hasUniform(Uniforms::Camera_Position)) {
			glm::vec3 tmp = glm::inverse(cameraView)[3];
			glUniform3f(shdr->getUniformID(Uniforms::Camera_Position), tmp.x, tmp.y, tmp.z);
		}

		if (shdr->hasUniform(Uniforms::Bones)) {
			glUniformMatrix4fv(shdr->getUniformID(Uniforms::Bones), 1, GL_FALSE, &model->getSkeleton()->getJointTransforms()[0][0][0]);
		}


		//Bind the model's vertex array id
		glBindVertexArray(model->getVertexArrayID());//Bind the model's vertex array id

		if (model->usesIndeces()) {
			glDrawElements(model->getDrawMode(), model->getNumIndeces(), GL_UNSIGNED_INT, 0);
		}
		else {
			//Draw the model's points from the currently bound VAO with currently used shader
			glDrawArrays(model->getDrawMode(), 0, model->getVertCount());
		}

		glBindVertexArray(0);

		model->getMaterial()->unuseMaterial();
	}
}

void pRenderer::setSkybox(pCubeMap * cubeMap)
{
	skybox = cubeMap;
}

void pRenderer::renderSkybox()
{
	//Store the culling and depth modes for temporary change
	GLint lastCullFaceMode;
	GLint lastDepthFuncMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &lastCullFaceMode);
	glGetIntegerv(GL_DEPTH_FUNC, &lastDepthFuncMode);

	//Set the culling and depth settings for skybox rendering
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	pModel* model = skybox->getModel();
	pShader* shdr = model->getMaterial()->getShader();

	glUseProgram(shdr->getShaderID());

	skybox->UseTexture(GL_TEXTURE0);

	glm::mat4 skyboxMVP = projMatrix * glm::inverse(glm::transpose(cameraView)) * skybox->getTransform();

	//Apply the model position matrix
	if (shdr->hasUniform(Uniforms::Model_View)) {
		glUniformMatrix4fv(model->getModelMatrixID(), 1, GL_FALSE, &skybox->getTransform()[0][0]);
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
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(skybox->getTransform()));
		glUniformMatrix3fv(model->getNormalMatrixID(), 1, GL_FALSE, &normalMatrix[0][0]);
	}

	if (shdr->hasUniform(Uniforms::ModelViewProjection)) {
		glUniformMatrix4fv(model->getMVPMatrixID(), 1, GL_FALSE, &skyboxMVP[0][0]);
	}

	//Feed the model the camera position
	if (shdr->hasUniform(Uniforms::Camera_Position)) {
		glUniform3f(shdr->getUniformID(Uniforms::Camera_Position), camLoc.x, camLoc.y, camLoc.z);
	}

	//Feed the cube map texture
	if (shdr->hasUniform(Uniforms::Cube_Map_Texture)) {
		
	}


	//Bind the model's vertex array id
	glBindVertexArray(model->getVertexArrayID());//Bind the model's vertex array id

	if (model->usesIndeces()) {
		glDrawElements(model->getDrawMode(), model->getNumIndeces(), GL_UNSIGNED_INT, 0);
	}
	else {
		//Draw the model's points from the currently bound VAO with currently used shader
		glDrawArrays(model->getDrawMode(), 0, model->getVertCount());
	}

	glBindVertexArray(0);

	skybox->UnuseTexture(GL_TEXTURE0);
	glUseProgram(0);

	//Set the culling and depth settings back to previous state
	glCullFace(lastCullFaceMode);
	glDepthFunc(lastDepthFuncMode);
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

void pRenderer::setProjectionMatrix(glm::mat4 projMatrix)
{
	this->projMatrix = projMatrix;
}

void pRenderer::setViewMatrix(glm::mat4 viewMatrix)
{
	this->cameraView = viewMatrix;
}

void pRenderer::initDefaultView()
{
	setViewMatrix(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	setProjectionMatrix(-0.5f, 0.5f, -0.5f, 0.5f, 0.2f, 10.0f);
}
