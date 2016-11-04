#include "pModel.h"
#include "pFileReader.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include <iostream>
#include <vector>

pModel::pModel(std::string name, pMaterial * material, GLfloat * verts, GLfloat * vertColors, GLfloat* vertUVs, GLfloat* vertNorms, GLuint numVertices, GLenum drawMode)
{
	this->name = name;
	this->type = pType::MODEL;
	this->material = material;
	this->drawMode = drawMode;
	vertCount = numVertices;

	//Allocate the required memory for the model's vertices (each vertex is 3 floats, so #verts * 3 * size of a float)
	//Could be made more convenient with a vertex struct
	vertices = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

	//Copy over the verticies array (numVertices * 3 because there are 3x locations per vertex, again would be better with a vertex struct)
	for (GLuint x(0); x < (numVertices * 3); ++x) {
		vertices[x] = verts[x];
	}

	//Allocate memory for the colors
	vertexColors = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

	//Copy the incoming color info to the vertexColors member
	for (GLuint x(0); x < (numVertices * 3); ++x) {
		vertexColors[x] = vertColors[x];
	}

	//Allocate memory for the UVs
	vertexUVs = (GLfloat*)malloc(numVertices * 2 * sizeof(GLfloat));

	//Copy the incoming UV info to the vertexUVs member
	for (GLuint x(0); x < (numVertices * 2); ++x) {
		vertexUVs[x] = vertUVs[x];
	}

	//Allocate memory for the normals
	vertexNormals = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

	//Copy incoming normal info into the vertexNormals member
	for (GLuint x(0); x < (numVertices * 3); ++x) {
		vertexNormals[x] = vertNorms[x];
	}

	//Set the position and rotational matrices to 0
	initDefaultMatrix();
	//Create the buffer objects
	setupModel();
}

pModel::~pModel()
{
	delete vertices;
	delete vertexColors;
	delete vertexUVs;
}

std::string pModel::getName()
{
	return name;
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

GLenum pModel::getDrawMode()
{
	return drawMode;
}

void pModel::rotateAround(glm::vec3 rot, GLfloat amount)
{
	rotationMatrix *= glm::rotate(glm::radians(amount), rot);
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

void pModel::setRotation(glm::vec3 rot, GLfloat amount)
{
	rotationMatrix = glm::rotate(glm::radians(amount), rot);
}


void pModel::setupModel()
{
	//Generate the points VBO, bind it and copy the points onto the buffer
	VBOID[0] = 0;
	glGenBuffers(1, &VBOID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

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

	//Generate Normals VBO, bind it and copy the points onto the buffer
	VBOID[3] = 0;
	glGenBuffers(1, &VBOID[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[3]);
	glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), vertexNormals, GL_STATIC_DRAW);

	//Create the vertex array object to hold our VBOs and bind it
	glGenVertexArrays(1, &VAOID);
	glBindVertexArray(VAOID);
	//Bind our first (point) buffer to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[0]);
	//Bind the vPosition shader attribute to the 0th VBO (points)
	glBindAttribLocation(getShaderProgramID(), 0, material->getShaderInfo().vertexPositionAttribute);
	//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Bind our second (color) buffer to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[1]);
	//Bind the vColor shader attribute to the 2nd VBO (color)
	glBindAttribLocation(getShaderProgramID(), 1, material->getShaderInfo().vertexColorAttribute);
	//Attirbute pointer to the 1st index, 3 of type, type is float, not normalized, 0 stride, no pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Bind our third (UV) buffer to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[2]);
	//Bind the vColor shader attribute to the 3rd VBO (UV)
	glBindAttribLocation(getShaderProgramID(), 1, material->getShaderInfo().vertexColorAttribute);
	//Attirbute pointer to the 1st index, 2 of type, type is float, not normalized, 0 stride, no pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//Bind our fourth (Normal) buffer to the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOID[3]);
	glBindAttribLocation(getShaderProgramID(), 1, material->getShaderInfo().vertexNormalAttribute);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Enable the vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	modelMatrixLocation = glGetUniformLocation(getShaderProgramID(), material->getShaderInfo().modelViewAttribute);
	viewMatrixLocation = glGetUniformLocation(getShaderProgramID(), material->getShaderInfo().viewAttribute);
	projectionMatrixLocation = glGetUniformLocation(getShaderProgramID(), material->getShaderInfo().projectionAttribute);

	material->setupTexture();
}

void pModel::initDefaultMatrix()
{
	rotationMatrix = glm::mat4(1.0f);
	translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
}
