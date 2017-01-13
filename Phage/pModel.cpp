#include "pModel.h"
#include "pFileReader.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "GLError.h"
#include <iostream>
#include <vector>


pModel::pModel(std::string name, pMaterial * material, GLenum drawMode, GLuint numVerts, GLfloat * vPositions, GLuint numIndeces, GLuint* vIndeces, GLfloat * vCoordinates, GLfloat * vNormals, GLfloat * vTangents, GLfloat * vBiTangents, GLfloat * vColors)
{
	if (name == "") { name = (material->getName() + "NewModel"); }
	this->name = name;
	this->drawMode = drawMode;
	this->material = material;
	this->drawMode = drawMode;

	vertCount = numVerts;
	numIndeces = numIndeces;

	useIndeces = !(vIndeces == nullptr);
	useVertices = !(vPositions == nullptr);
	useTexCoords = !(vCoordinates == nullptr);
	useNormals = !(vNormals == nullptr);
	useTangents = !(vTangents == nullptr);
	useBiTangents = !(vBiTangents == nullptr);
	useColors = !(vColors == nullptr);

	if (useIndeces) {
		this->vIndeces = std::vector<GLuint>();
		for (int i = 0; i < numIndeces; ++i) {
			this->vIndeces.push_back(vIndeces[i]);
		}
	}

	if (useVertices) {
		this->vPositions = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		for (int i = 0; i < totalFloats; ++i) {
			this->vPositions.push_back(vPositions[i]);
		}
	}

	if (useTexCoords) {
		this->vCoordinates = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 2);
		for (int i = 0; i < totalFloats; ++i) {
			this->vCoordinates.push_back(vCoordinates[i]);
		}
	}

	if (useNormals) {
		this->vNormals = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		for (int i = 0; i < totalFloats; ++i) {
			this->vNormals.push_back(vNormals[i]);
		}
	}

	if (useTangents) {
		this->vTangents = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		for (int i = 0; i < totalFloats; ++i) {
			this->vTangents.push_back(vTangents[i]);
		}
	}

	if (useBiTangents) {
		this->vBiTangents = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		for (int i = 0; i < totalFloats; ++i) {
			this->vBiTangents.push_back(vBiTangents[i]);
		}
	}

	if (useColors) {
		this->vColors = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		for (int i = 0; i < totalFloats; ++i) {
			this->vColors.push_back(vColors[i]);
		}
	}

	initDefaultMatrix();

	setupModel();

}

pModel::pModel(std::string name, pMaterial * material, GLenum drawMode, std::vector<GLfloat> vPositions, 
	std::vector<GLuint> vIndeces = std::vector<GLuint>(), 
	std::vector<GLfloat> vCoordinates = std::vector<GLfloat>(), 
	std::vector<GLfloat> vNormals = std::vector<GLfloat>(), 
	std::vector<GLfloat> vTangents = std::vector<GLfloat>(),
	std::vector<GLfloat> vBiTangents = std::vector<GLfloat>(),
	std::vector<GLfloat> vColors = std::vector<GLfloat>())
{
	this->name = name;
	this->material = material;
	this->drawMode = drawMode;

	this->vertCount = vPositions.size();
	this->numIndeces = vIndeces.size();

	useIndeces = false;
	useVertices = false;
	useTexCoords = false;
	useNormals = false;
	useTangents = false;
	useBiTangents = false;
	useColors = false;

	if (vertCount > 0) { 
		useVertices = true;
		this->vPositions = std::vector<GLfloat>(vPositions);

		if (numIndeces > 0) {
			useIndeces = true;
			this->vIndeces = std::vector<GLuint>(vIndeces);
		}

		if (vCoordinates.size() > 0) {
			useTexCoords = true;
			this->vCoordinates = std::vector<GLfloat>(vCoordinates);
		}

		if (vNormals.size() > 0) {
			useNormals = true;
			this->vNormals = std::vector<GLfloat>(vNormals);
		}

		if (vTangents.size() > 0) {
			useTangents = true;
			this->vTangents = std::vector<GLfloat>(vTangents);
		}

		if (vBiTangents.size() > 0) {
			useBiTangents = true;
			this->vBiTangents = std::vector<GLfloat>(vBiTangents);
		}

		if (vColors.size() > 0) {
			useColors = true;
			this->vColors = std::vector<GLfloat>(vColors);
		}
	}

	initDefaultMatrix();

	setupModel();
}

pModel::~pModel()
{

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

GLuint pModel::getElementBufferID()
{
	return EBO;
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

void pModel::setMaterial(pMaterial * newMaterial)
{
	material = newMaterial;
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

bool pModel::usesIndeces()
{
	return useIndeces;
}

GLuint pModel::getNumIndeces()
{
	return numIndeces;
}


void pModel::setupModel()
{
	int currentBuffer = 0;

	pShader* shader = material->getShader();
	
	GLError::printError(__FILE__, __LINE__);

	useVertices = shader->hasAttribute(Attributes::VertexPosition);
	useColors = shader->hasAttribute(Attributes::VertexColor);
	useNormals = shader->hasAttribute(Attributes::VertexNormal);
	useTexCoords = shader->hasAttribute(Attributes::VertexCoordinate);

	glGenVertexArrays(1, &VAOID);

	glBindVertexArray(VAOID);

	//Set up the element array buffer if indeces are availible
	if (useIndeces) {
		EBO = 0;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndeces, &vIndeces[0], GL_STATIC_DRAW);
	}

	if (useVertices && (shader->hasAttribute(Attributes::VertexPosition))) {
		//Generate the points VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vPositions[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexPosition);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		currentBuffer++;
	}

	if (useColors && (shader->hasAttribute(Attributes::VertexColor))) {
		//Generate the colors VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vColors[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexColor);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		currentBuffer++;
	}


	if (useTexCoords && (shader->hasAttribute(Attributes::VertexCoordinate))) {
		//Generate the UV VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 2 * sizeof(GLfloat), &vCoordinates[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 2 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexCoordinate);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		currentBuffer++;
	}


	if (useNormals && (shader->hasAttribute(Attributes::VertexNormal))) {
		//Generate Normals VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vNormals[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexNormal);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		currentBuffer++;
	}

	if (useTangents && (shader->hasAttribute(Attributes::VertexTangent))) {
		//Generate Normals VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vTangents[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexTangent);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		currentBuffer++;
	}

	if (useBiTangents && (shader->hasAttribute(Attributes::VertexBiTangent))) {
		//Generate Normals VBO, bind it and copy the points onto the buffer
		VBOID[currentBuffer] = 0;
		glGenBuffers(1, &VBOID[currentBuffer]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOID[currentBuffer]);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vBiTangents[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexBiTangent);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		
		currentBuffer++;
	}


	modelMatrixLocation = glGetUniformLocation(getShaderProgramID(), material->getShaderInfo().modelViewAttribute);
	viewMatrixLocation = glGetUniformLocation(getShaderProgramID(), material->getShaderInfo().viewAttribute);
	projectionMatrixLocation = glGetUniformLocation(getShaderProgramID(), material->getShaderInfo().projectionAttribute);

	if (material != NULL)
	{
		material->setupTexture();
	}
}

void pModel::initDefaultMatrix()
{
	rotationMatrix = glm::mat4(1.0f);
	translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
}
