#include "pModel.h"
#include "pFileReader.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "GLError.h"
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

	//Vertex data flags set false by default
	useVertices = false;
	useTexCoords = false;
	useNormals = false;
	useColors = false;

	if (verts != nullptr) {
		useVertices = true;

		//Copy over the verticies array (numVertices * 3 because there are 3x locations per vertex, again would be better with a vertex struct)
		for (GLuint x(0); x < (numVertices * 3); ++x) {
			vertices[x] = verts[x];
		}
	}

	if (vertColors != nullptr) {
		useColors = true;
		//Allocate memory for the colors
		vertexColors = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

		//Copy the incoming color info to the vertexColors member
		for (GLuint x(0); x < (numVertices * 3); ++x) {
			vertexColors[x] = vertColors[x];
		}
	}

	if (vertUVs != nullptr) {
		useTexCoords = true;
		//Allocate memory for the UVs
		vertexUVs = (GLfloat*)malloc(numVertices * 2 * sizeof(GLfloat));

		//Copy the incoming UV info to the vertexUVs member
		for (GLuint x(0); x < (numVertices * 2); ++x) {
			vertexUVs[x] = vertUVs[x];
		}
	}

	if (vertNorms != nullptr) {
		useNormals = true;
		//Allocate memory for the normals
		vertexNormals = (GLfloat*)malloc(numVertices * 3 * sizeof(GLfloat));

		//Copy incoming normal info into the vertexNormals member
		for (GLuint x(0); x < (numVertices * 3); ++x) {
			vertexNormals[x] = vertNorms[x];
		}
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
	delete vertexNormals;
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
	return material->getModelMatrixID();
}

GLuint pModel::getViewMatrixID()
{
	return material->getViewMatrixID();
}

GLuint pModel::getProjectionMatrixID()
{
	return material->getProjectionMatrixID();
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

void pModel::UseMaterial()
{
	material->useMaterial();
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
	int currentBuffer = 0;

	pShader* shader = material->getShader();
	
	//useVertices = shader->hasAttribute(Attributes::VertexPosition);
	//useColors = shader->hasAttribute(Attributes::VertexColor);
	//useNormals = shader->hasAttribute(Attributes::VertexNormal);
	//useTexCoords = shader->hasAttribute(Attributes::VertexCoordinate);

	//Create the vertex array object to hold our VBOs and bind it
	glGenVertexArrays(1, &VAOID);
	glBindVertexArray(VAOID);

	

	if (useVertices && (shader->hasAttribute(Attributes::VertexPosition))) {
		//Generate the points VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexPosition);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attribID);

		currentBuffer++;
	}

	

	if (useColors && (shader->hasAttribute(Attributes::VertexColor))) {
		//Generate the colors VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), vertexColors, GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexColor);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attribID);

		currentBuffer++;
	}

	

	if (useTexCoords && (shader->hasAttribute(Attributes::VertexCoordinate))) {
		//Generate the UV VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 2 * sizeof(GLfloat), vertexUVs, GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 2 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexCoordinate);
		glVertexAttribPointer(attribID, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attribID);
		currentBuffer++;
	}

	

	if (useNormals && (shader->hasAttribute(Attributes::VertexNormal))) {
		//Generate Normals VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), vertexNormals, GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexNormal);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(attribID);
	}

	
	
}

void pModel::initDefaultMatrix()
{
	rotationMatrix = glm::mat4(1.0f);
	translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
}
