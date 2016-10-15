#include "pRenderer.h"
const GLfloat bg[] = { 0.0f, 0.0f, 0.0f, 0.0f };

pRenderer::pRenderer(GLFWwindow* window)
{
	setWindowRef(window);
	setupGLOptions();
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
	/*
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->vertices), model->vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glDrawArrays(GL_QUADS, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);*/

	glClearBufferfv(GL_COLOR, 0, bg);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(0.5, 0, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0, 1.0, 0);
	glEnd();
}
