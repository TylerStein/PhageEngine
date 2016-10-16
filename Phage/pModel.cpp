#include "pModel.h"
#include "pFileReader.h"
#include <iostream>
#include <vector>

pModel::pModel(char * name, pMaterial * material, GLfloat * verts, GLfloat * vertColors, GLuint numVertices) : pAsset(pType::MODEL)
{
	this->name = name;
	this->type = pType::MODEL;
	this->material = material;
	vertCount = numVertices;

	//Allocate the required memory for the model's vertices (each vertex is 3 floats, so #verts * 3 * size of a float)
	//Could be made more convenient with a vertex struct
	vertices = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

	//Copy over the verticies array (numVertices * 3 because there are 3x locations per vertex, again would be better with a vertex struct)
	for (GLuint x(0); x < (numVertices * 3); ++x) {
		vertices[x] = verts[x];
	}

	vertexColors = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

	for (GLuint x(0); x < (numVertices * 3); ++x) {
		vertexColors[x] = vertColors[x];
	}

	setupModel();
}

pModel::~pModel()
{
	delete vertices;
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
	return vertexArrayID;
}

GLuint pModel::getVertexBufferID()
{
	return vertexBufferID;
}

void pModel::setupModel()
{
	//Create the vertex buffer object
	vertexBufferID = 0;
	//Generate the vbo
	glGenBuffers(1, &vertexBufferID);
	//Set buffer as the current one by binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	//Copy the points into the bound buffer
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Create vertex array object
	vertexArrayID = 0;
	//Generate the vao
	glGenVertexArrays(1, &vertexArrayID);
	//Set the vao as the current one
	glBindVertexArray(vertexArrayID);
	//Only one vertex buffer, so attribute 0
	glEnableVertexAttribArray(0);
	//Bind that buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayID);
	//Define the layout for attribute number 0,
	//3 means the variables are vec3 made from 3 floats in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	material->loadShader("../Resources/Shaders/simpleVertexShader.vert", "../Resources/Shaders/simpleFragmentShader.frag");
}
