#include "pModel.h"
#include "pFileReader.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include <iostream>
#include <vector>

pModel::pModel(char * name, pMaterial * material, GLfloat * vertPositions,  GLfloat* vertNormals, GLfloat * vertColors, GLfloat* vertUVs, GLuint numVertices)
{
	this->name = name;
	this->type = pType::MODEL;
	this->material = material;
	vertCount = numVertices;

	//Allocate the required memory for the model's vertex information (number of vertices * 3(x,y,z) * size of one GLfloat)
	vertexPositions = (GLfloat*)malloc(vertCount * 3 * sizeof(GLfloat));
	vertexNormals = (GLfloat*)malloc(vertCount * 3 * sizeof(GLfloat));
	vertexUVs = (GLfloat*)malloc(vertCount * 3 * sizeof(GLfloat));
	vertexColors = (GLfloat*)malloc(vertCount * 3 * sizeof(GLfloat));

	//Copy over vertex positions
	for (GLuint x(0); x < (vertCount); ++x) {
		vertexPositions[x] = vertPositions[x];
	}

	//Copy over vertex colors
	for (GLuint x(0); x < (vertCount); ++x) {
		vertexColors[x] = vertColors[x];
	}

	//Copy over vertex normals
	for (GLuint x(0); x < (vertCount); ++x) {
		vertexNormals[x] = vertNormals[x];
	}

	//Copy over vertex UVs
	for (GLuint x(0); x < (vertCount); ++x) {
		vertexUVs[x] = vertUVs[x];
	}

	//Set the position and rotational matrices to 0
	initDefaultMatrix();
	//Create the buffer objects
	setupModel();
}

pModel::~pModel()
{
	delete vertexPositions;
	delete vertexNormals;
	delete vertexUVs;
	delete vertexColors;
}

std::string pModel::getName()
{
	return name;
}

GLuint pModel::getID()
{
	return ID;
}

GLuint pModel::getShaderProgramID()
{
	return material->getShaderProgramID();
}

GLuint pModel::getVertCount()
{
	return vertCount;
}

GLuint pModel::getVertexArrayID()
{
	return VAOID;
}

GLuint pModel::getVertexBufferID(GLuint index)
{
	return VBOID[index];
}

GLuint pModel::getModelMatrixID()
{
	return modelMatrixLocation;
}

GLuint pModel::getViewMatrixID()
{
	return viewMatrixLocation;
}

GLuint pModel::getProjectionMatrixID()
{
	return projectionMatrixLocation;
}

glm::mat4 pModel::getModelMatrix()
{
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	return modelMatrix;
}

pMaterial * pModel::getMaterial()
{
	return material;
}

void pModel::rotateAround(glm::vec3 rot, GLfloat amount)
{
	rotationMatrix = glm::rotate(amount, rot);
}

void pModel::scale(glm::vec3 scl)
{
	scaleMatrix = glm::scale(scaleMatrix, scl);
}

void pModel::setScale(glm::vec3 scl)
{
	scaleMatrix = glm::scale(glm::mat4(1.0f), scl);
}

void pModel::translate(glm::vec3 pos)
{
	translationMatrix = glm::translate(translationMatrix, pos);
}

void pModel::setPosition(glm::vec3 pos)
{
	translationMatrix = glm::translate(glm::mat4(1.0f), pos);
}


void pModel::setupModel()
{
	material->loadShader("../Resources/Shaders/simpleVertexShader.vert", "../Resources/Shaders/simpleFragmentShader.frag");

	//Generate the points VBO, bind it and copy the points onto the buffer
	VBOID[0] = 0;
	glGenBuffers(1, &VBOID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), vertexPositions, GL_STATIC_DRAW);

	//Generate the colors VBO, bind it and copy the points onto the buffer
	VBOID[1] = 0;
	glGenBuffers(1, &VBOID[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[1]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), vertexColors, GL_STATIC_DRAW);

	//Generate the UV VBO, bind it and copy the points onto the buffer
	VBOID[2] = 0;
	glGenBuffers(1, &VBOID[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[2]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * 2 * sizeof(GLfloat), vertexUVs, GL_STATIC_DRAW);

	//Create the vertex array object to hold our VBOs and bind it
	glGenVertexArrays(1, &VAOID);
	glBindVertexArray(VAOID);
	//Bind our first (point) buffer to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
	//Bind the vPosition shader attribute to the 0th VBO (points)
	glBindAttribLocation(getShaderProgramID(), 0, "vPosition");
	//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Bind our second (color) buffer to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[1]);
	//Bind the vColor shader attribute to the 1st VBO (color)
	glBindAttribLocation(getShaderProgramID(), 1, "vColor");
	//Attirbute pointer to the 1st index, 3 of type, type is float, not normalized, 0 stride, no pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Bind our third (UV) buffer to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[2]);
	//Bind the vColor shader attribute to the 1st VBO (color)
	glBindAttribLocation(getShaderProgramID(), 1, "vTexCoord");
	//Attirbute pointer to the 1st index, 2 of type, type is float, not normalized, 0 stride, no pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//Enable the vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	modelMatrixLocation = glGetUniformLocation(getShaderProgramID(), "model_matrix");
	viewMatrixLocation = glGetUniformLocation(getShaderProgramID(), "view_matrix");
	projectionMatrixLocation = glGetUniformLocation(getShaderProgramID(), "projection_matrix");
}

void pModel::initDefaultMatrix()
{
	rotationMatrix = glm::mat4(1.0f);
	translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
}
