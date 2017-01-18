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
	this->numIndeces = numIndeces;

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
	deleteBuffers();
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
	return ElementBufferID;
}

GLuint pModel::getProjectionMatrixID()
{
	return material->getProjectionMatrixID();
}

GLuint pModel::getNormalMatrixID()
{
	return material->getNormalMatrixID();
}

glm::mat4 pModel::getModelMatrix()
{
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	return modelMatrix;
}

glm::mat3 pModel::getNormalMatrix()
{
	glm::mat3 inverseModelView = glm::inverse(getModelMatrix());
	normalMatrix = glm::transpose(inverseModelView);
	return normalMatrix;
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

void pModel::setDrawMode(GLenum drawMode)
{
	this->drawMode = drawMode;
}

void pModel::scaleTextureCoordinates(glm::vec2 scale)
{
	//Ensure coordinates even exist
	if (&vCoordinates != nullptr) {
		//Scale each coordinate float
		for (int i(0); i < vertCount; i+=2) {
			vCoordinates[i] *= scale.x;
			vCoordinates[i + 1] *= scale.y;
		}

		//Overwrite the original coordinate buffer data
		glBindBuffer(GL_ARRAY_BUFFER, BufferID_Coordinates);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 2 * sizeof(GLfloat), &vCoordinates[0], GL_STATIC_DRAW);
	}

}

void pModel::deleteBuffers()
{
	//Deletes buffers from the GPU
	if(useVertices){ glDeleteBuffers(1, &BufferID_Positions); }
	if(useNormals) { glDeleteBuffers(1, &BufferID_Normals); }
	if (useColors) { glDeleteBuffers(1, &BufferID_Colors); }
	if (useTexCoords) { glDeleteBuffers(1, &BufferID_Coordinates); }
	if (useTangents) { glDeleteBuffers(1, &BufferID_Tangents); }
	if (useBiTangents) { glDeleteBuffers(1, &BufferID_BiTangents); }
	if (useIndeces) { glDeleteBuffers(1, &BufferID_Indeces); }
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


	//Create the vertex array object to hold our VBOs and bind it
	glGenVertexArrays(1, &VAOID);

	glBindVertexArray(VAOID);

	//Set up the element array buffer if indeces are availible
	if (useIndeces) {
		ElementBufferID = 0;
		glGenBuffers(1, &ElementBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndeces, &vIndeces[0], GL_STATIC_DRAW);
	}

	if (useVertices && (shader->hasAttribute(Attributes::VertexPosition))) {
		//Generate the points VBO, bind it and copy the points onto the buffer
		BufferID_Positions = 0;
		glGenBuffers(1, &BufferID_Positions);
		glBindBuffer(GL_ARRAY_BUFFER, BufferID_Positions);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vPositions[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexPosition);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	if (useColors && (shader->hasAttribute(Attributes::VertexColor))) {
		//Generate the colors VBO, bind it and copy the points onto the buffer
		BufferID_Colors = 0;
		glGenBuffers(1, &BufferID_Colors);
		glBindBuffer(GL_ARRAY_BUFFER, BufferID_Colors);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vColors[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexColor);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}


	if (useTexCoords && (shader->hasAttribute(Attributes::VertexCoordinate))) {
		//Generate the UV VBO, bind it and copy the points onto the buffer
		BufferID_Coordinates = 0;
		glGenBuffers(1, &BufferID_Coordinates);
		glBindBuffer(GL_ARRAY_BUFFER, BufferID_Coordinates);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 2 * sizeof(GLfloat), &vCoordinates[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 2 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexCoordinate);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}


	if (useNormals && (shader->hasAttribute(Attributes::VertexNormal))) {
		//Generate Normals VBO, bind it and copy the points onto the buffer
		BufferID_Normals = 0;
		glGenBuffers(1, &BufferID_Normals);
		glBindBuffer(GL_ARRAY_BUFFER, BufferID_Normals);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vNormals[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexNormal);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	if (useTangents && (shader->hasAttribute(Attributes::VertexTangent))) {
		//Generate Normals VBO, bind it and copy the points onto the buffer
		BufferID_Tangents = 0;
		glGenBuffers(1, &BufferID_Tangents);
		glBindBuffer(GL_ARRAY_BUFFER, BufferID_Tangents);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vTangents[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexTangent);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	if (useBiTangents && (shader->hasAttribute(Attributes::VertexBiTangent))) {
		//Generate Normals VBO, bind it and copy the points onto the buffer
		BufferID_BiTangents = 0;
		glGenBuffers(1, &BufferID_BiTangents);
		glBindBuffer(GL_ARRAY_BUFFER, BufferID_BiTangents);
		glBufferData(GL_ARRAY_BUFFER, vertCount * 3 * sizeof(GLfloat), &vBiTangents[0], GL_STATIC_DRAW);

		//Attirbute pointer to the 0th index, 3 of type, type is float, not normalized, 0 stride, no pointer
		GLuint attribID = shader->getAttributeID(Attributes::VertexBiTangent);
		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
}

void pModel::initDefaultMatrix()
{
	rotationMatrix = glm::mat4(1.0f);
	translationMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	modelMatrix = glm::mat4(1.0f);
}
