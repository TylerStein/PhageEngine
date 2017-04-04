#pragma once
#include "GL\glew.h"
#include <string>
#include "pAsset.h"
#include "pMaterial.h"
#include "glm\mat4x4.hpp"
#include "pSkeleton.h"

struct Vertex {
	Vertex(glm::vec3 pos = glm::vec3(0), glm::vec3 norm = glm::vec3(0), glm::vec2 coord = glm::vec2(0), glm::vec3 tan = glm::vec3(0), glm::vec3 bitan = glm::vec3(0), glm::vec3 col = glm::vec3(0)) {
		position = pos;
		normal = norm;
		coordinates = coord;
		tangent = tan;
		biTangent = bitan;
		color = col;
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 coordinates;
	glm::vec3 tangent;
	glm::vec3 biTangent;
	glm::vec3 color;
};

class pModel : public pAsset
{

	friend class pBoundingBox;

public:
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

	Skeleton* getSkeleton() const;
	void setSkeleton(Skeleton* skeleton);

	pType type;

	std::string directory;

	std::vector<glm::vec3> getVertexPositions();

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

	bool useBones;

	pMaterial* material;
	
	//This model's skeleton (can be nullptr)
	Skeleton* skeleton;

	GLuint vertCount;

	GLuint numIndeces;
	GLuint ElementBufferID; //Potentially uses an element buffer (for indexed vertices)

	GLuint EBOID; //Element buffer object ID
	GLuint VAOID; //Vertex array object ID
	GLuint VBOID; //Vertex buffer object ID

	std::vector<Vertex> vertices;
	std::vector<GLuint> vIndeces;
	

	GLenum drawMode;
};

