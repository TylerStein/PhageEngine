#include "pModel.h"
#include "pFileReader.h"
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"
#include "GLError.h"
#include <iostream>
#include <vector>
#include "GLFW\glfw3.h"
#include "Shader.h"

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

	vertices = std::vector<Vertex>(vertCount);

	if (useIndeces) {
		this->vIndeces = std::vector<GLuint>();
		for (int i = 0; i < numIndeces; ++i) {
			this->vIndeces.push_back(vIndeces[i]);
		}
	}

	if (useVertices) {
		//this->vPositions = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		//for (int i = 0; i < totalFloats; ++i) {
		//	this->vPositions.push_back(vPositions[i]);
		//}

		int v = 0;
		for (int i = 0; i < totalFloats; i += 3) {
			vertices[v].position = glm::vec3(vPositions[i], vPositions[i + 1], vPositions[i + 2]);
			v++;
		}

	}

	if (useTexCoords) {
		//this->vCoordinates = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 2);
		//for (int i = 0; i < totalFloats; ++i) {
		//	this->vCoordinates.push_back(vCoordinates[i]);
		//}

		int v = 0;
		for (int i = 0; i < totalFloats; i += 2) {
			vertices[v].coordinates = glm::vec2(vCoordinates[i], vCoordinates[i + 1]);
			v++;
		}
	}

	if (useNormals) {
		//this->vNormals = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		//for (int i = 0; i < totalFloats; ++i) {
		//	this->vNormals.push_back(vNormals[i]);
		//}

		int v = 0;
		for (int i = 0; i < totalFloats; i += 3) {
			vertices[v].normal = glm::vec3(vNormals[i], vNormals[i + 1], vNormals[i + 2]);
			v++;
		}
	}

	if (useTangents) {
		//this->vTangents = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		//for (int i = 0; i < totalFloats; ++i) {
		//	this->vTangents.push_back(vTangents[i]);
		//}

		int v = 0;
		for (int i = 0; i < totalFloats; i += 3) {
			vertices[v].tangent = glm::vec3(vTangents[i], vTangents[i + 1], vTangents[i + 2]);
			v++;
		}
	}

	if (useBiTangents) {
		//this->vBiTangents = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		//for (int i = 0; i < totalFloats; ++i) {
		//	this->vBiTangents.push_back(vBiTangents[i]);
		//}

		int v = 0;
		for (int i = 0; i < totalFloats; i += 3) {
			vertices[v].biTangent = glm::vec3(vBiTangents[i], vBiTangents[i + 1], vBiTangents[i + 2]);
			v++;
		}
	}

	if (useColors) {
		//this->vColors = std::vector<GLfloat>();
		const GLuint totalFloats = (numVerts * 3);
		//for (int i = 0; i < totalFloats; ++i) {
		//	this->vColors.push_back(vColors[i]);
		//}

		int v = 0;
		for (int i = 0; i < totalFloats; i += 3) {
			vertices[v].color = glm::vec3(vColors[i], vColors[i + 1], vColors[i + 2]);
			v++;
		}
	}

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

		vertices = std::vector<Vertex>(vertCount);

		if (vPositions.size() > 0) {
			useVertices = true;
			//this->vPositions = std::vector<GLfloat>(vPositions);

			int v = 0;
			for (int i = 0; i < vPositions.size(); i += 3) {
				vertices[v].position = glm::vec3(vPositions[i], vPositions[i + 1], vPositions[i + 2]);
				v++;
			}

		}

		if (numIndeces > 0) {
			useIndeces = true;
			this->vIndeces = std::vector<GLuint>(vIndeces);
		}

		if (vCoordinates.size() > 0) {
			useTexCoords = true;
			//this->vCoordinates = std::vector<GLfloat>(vCoordinates);

			int v = 0;
			for (int i = 0; i < vCoordinates.size(); i += 2) {
				vertices[v].coordinates = glm::vec2(vCoordinates[i], vCoordinates[i + 1]);
				v++;
			}
		}

		if (vNormals.size() > 0) {
			useNormals = true;
			//this->vNormals = std::vector<GLfloat>(vNormals);

			int v = 0;
			for (int i = 0; i < vNormals.size(); i += 3) {
				vertices[v].normal = glm::vec3(vNormals[i], vNormals[i + 1], vNormals[i + 2]);
				v++;
			}
		}

		if (vTangents.size() > 0) {
			useTangents = true;
			//this->vTangents = std::vector<GLfloat>(vTangents);

			int v = 0;
			for (int i = 0; i < vTangents.size(); i += 3) {
				vertices[v].tangent = glm::vec3(vTangents[i], vTangents[i + 1], vTangents[i + 2]);
				v++;
			}
		}

		if (vBiTangents.size() > 0) {
			useBiTangents = true;
			//this->vBiTangents = std::vector<GLfloat>(vBiTangents);

			int v = 0;
			for (int i = 0; i < vBiTangents.size(); i += 3) {
				vertices[v].biTangent = glm::vec3(vBiTangents[i], vBiTangents[i + 1], vBiTangents[i + 2]);
				v++;
			}
		}

		if (vColors.size() > 0) {
			useColors = true;
			//this->vColors = std::vector<GLfloat>(vColors);

			int v = 0;
			for (int i = 0; i < vColors.size(); i += 3) {
				vertices[v].color = glm::vec3(vColors[i], vColors[i + 1], vColors[i + 2]);
				v++;
			}
		}
	}

	setupModel();
}

pModel::~pModel()
{
	deleteBuffers();
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
	return EBOID;
}

GLuint pModel::getProjectionMatrixID()
{
	return material->getProjectionMatrixID();
}

GLuint pModel::getNormalMatrixID()
{
	return material->getNormalMatrixID();
}

GLuint pModel::getMVPMatrixID()
{
	return material->getMVPMatrixID();
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

void pModel::setDrawMode(GLenum drawMode)
{
	this->drawMode = drawMode;
}

void pModel::scaleTextureCoordinates(glm::vec2 scale)
{
	//Delete the old buffers
	deleteBuffers();

	//Scale the texture coords
	for (int i = 0; i < vertCount; i++) {
		vertices[i].coordinates *= scale;
	}

	//Remake the model data buffer
	setupModel();
}
void pModel::deleteBuffers()
{
	glDeleteVertexArrays(1, &VAOID);
	glDeleteBuffers(1, &VBOID);
	glDeleteBuffers(1, &EBOID);
}

bool pModel::usesIndeces()
{
	return useIndeces;
}

GLuint pModel::getNumIndeces()
{
	return numIndeces;
}

std::vector<glm::vec3> pModel::getVertexPositions()
{
	std::vector<glm::vec3> vPos = std::vector<glm::vec3>(vertCount);
	for (int i = 0; i < vertCount; i++) {
		vPos[i] = vertices[i].position;
	}

	return vPos;
}

void pModel::setupModel() {
	pShader* shader = material->getShader();

	Shader* shdr = new Shader(
		std::vector<std::string>() = { "../Resources/Shaders/phong.vert", "../Resources/Shaders/phong.frag" },
		std::vector<GLenum>() = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER },
		std::vector<ShaderValue>() = {
		ShaderValue("vPositions", 0, GL_FLOAT_VEC3, VertexPosition),
		ShaderValue("vCoordinates", 0, GL_FLOAT_VEC2, VertexCoordinate),
		ShaderValue("vNormals", 0, GL_FLOAT_VEC3, VertexNormal)
	});

	glGenVertexArrays(1, &VAOID);
	glGenBuffers(1, &VBOID);
	glGenBuffers(1, &EBOID);

	glBindVertexArray(VAOID);
	

	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	

	ShaderValue* positionValue = shdr->getShaderAttributeValue(VertexPosition);
	shdr->sendShaderAttribute(*positionValue, VBOID, 0U, GL_STATIC_DRAW, GL_VERTEX_ARRAY);

	ShaderValue* elementValue = shdr->getShaderAttributeValue(VertexIndeces);
	shdr->sendShaderAttribute(*elementValue, EBOID, 0U, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);
	

	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndeces * sizeof(GLuint), vIndeces.data(), GL_STATIC_DRAW);

	//Vertex attributes
	//Position
	if (useVertices && shader->hasAttribute(Attributes::VertexPosition)) {
		GLuint attribID = shader->getAttributeID(Attributes::VertexPosition);

		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	}

	//Normals
	if (useNormals && shader->hasAttribute(Attributes::VertexNormal)) {
		GLuint attribID = shader->getAttributeID(Attributes::VertexNormal);

		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	}

	//Coordinates
	if (useTexCoords && shader->hasAttribute(Attributes::VertexCoordinate)) {
		GLuint attribID = shader->getAttributeID(Attributes::VertexCoordinate);

		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, coordinates));
	}

	//Tangents
	if (useTangents && shader->hasAttribute(Attributes::VertexTangent)) {
		GLuint attribID = shader->getAttributeID(Attributes::VertexTangent);

		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));
	}

	//Bitangents
	if (useBiTangents && shader->hasAttribute(Attributes::VertexBiTangent)) {
		GLuint attribID = shader->getAttributeID(Attributes::VertexBiTangent);

		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, biTangent));
	}

	//Colors
	if (useColors && shader->hasAttribute(Attributes::VertexColor)) {
		GLuint attribID = shader->getAttributeID(Attributes::VertexColor);

		glEnableVertexAttribArray(attribID);
		glVertexAttribPointer(attribID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	}

	//Bones
	//if (useBones && shader->hasAttribute(Attributes::BoneData)) {
	//	GLuint attribID = shader->getAttributeID(Attributes::BoneData);
	//
	//	glEnableVertexAttribArray(attribID);
	//	glVertexAttribIPointer(attribID, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)offsetof(Vertex, boneData));
	//}

}