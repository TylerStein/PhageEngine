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
};

