#pragma once
#include "GL\glew.h"
#include <string>
#include "pAsset.h"
#include "pMaterial.h"
#include "glm\mat4x4.hpp"

class pModel : public pAsset
{

	friend class pBoundingBox;

public:
	enum Buffers {POSITIONS, NORMALS, TEXCOORDS, INDECES, COLORS, TANGENTS, BITANGENTS };
	//pModel(std::string name, pMaterial* material, GLfloat* verts, GLfloat* vertColors, GLfloat* vertUVs, GLfloat* vertNorms, GLuint numVertices, GLenum drawMode);
	pModel(std::string name, pMaterial* material, GLenum drawMode, GLuint numVerts, GLfloat* vPositions, GLuint numIndeces = 0, GLuint* vIndeces = nullptr, GLfloat* vCoordinates = nullptr, GLfloat* vNormals = nullptr, GLfloat* vTangents = nullptr, GLfloat* vBiTangents = nullptr, GLfloat* vColors = nullptr);
	pModel(std::string name, pMaterial* material, GLenum drawMode, std::vector<GLfloat> vPositions, std::vector<GLuint> vIndeces, std::vector<GLfloat> vCoordinates, std::vector<GLfloat> vNormals, std::vector<GLfloat> vTangents, std::vector<GLfloat> vBiTangents, std::vector<GLfloat> vColors);
	~pModel();

	GLuint getShaderProgramID();
	GLuint getVertCount();
	GLuint getVertexArrayID();
	GLuint getModelMatrixID();
	GLuint getViewMatrixID();
	GLuint getElementBufferID();
	GLuint getProjectionMatrixID();
	GLuint getNormalMatrixID();
	GLuint getMVPMatrixID();

	glm::mat4 getModelMatrix();
	glm::mat3 getNormalMatrix();

	pMaterial* getMaterial();
	void setMaterial(pMaterial* newMaterial);

	GLenum getDrawMode();

	void UseMaterial();

	//Adds rotation relative to current around a provided axis by <amount> degrees
	void rotateAround(glm::vec3 rot, GLfloat amount);
	//Scales relative to current scale by <xyz>
	void scale(glm::vec3 scl);
	//Sets the scale (not relative) by <xyz>
	void setScale(glm::vec3 scl);
	//Translates relative to current position by <xyz>
	void translate(glm::vec3 pos);
	//Translates relative to center by <xyz>
	void setPosition(glm::vec3 pos);
	//Set the rotation relative to 0
	void setRotation(glm::vec3 rot, GLfloat amount);
	//Set the draw mode
	void setDrawMode(GLenum drawMode);
	//Scale the UV
	void scaleTextureCoordinates(glm::vec2 scale);

	void deleteBuffers();

	bool usesIndeces();
	GLuint getNumIndeces();

	pType type;

private:
	//Function for initializing the VBO, VAO and ShaderProgram
	void setupModel();
	void initDefaultMatrix();

	bool useVertices;
	bool useNormals;
	bool useTexCoords;
	bool useColors;
	bool useTangents;
	bool useBiTangents;
	bool useIndeces;

	std::vector<GLuint> vIndeces;

	std::vector<GLfloat> vPositions;
	std::vector<GLfloat> vNormals;
	std::vector<GLfloat> vCoordinates;
	std::vector<GLfloat> vColors;
	std::vector<GLfloat> vTangents;
	std::vector<GLfloat> vBiTangents;

	pMaterial* material;

	GLuint vertCount;

	GLuint numIndeces;
	GLuint VAOID; //One vertex array object to hold the vertex buffer objects
	GLuint ElementBufferID; //Potentially uses an element buffer (for indexed vertices)

	GLuint BufferID_Positions;
	GLuint BufferID_Normals;
	GLuint BufferID_Coordinates;
	GLuint BufferID_Colors;
	GLuint BufferID_Indeces;
	GLuint BufferID_Tangents;
	GLuint BufferID_BiTangents;

	GLenum drawMode;

	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;

	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
};

